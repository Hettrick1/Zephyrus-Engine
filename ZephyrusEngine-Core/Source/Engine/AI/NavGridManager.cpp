#include "pch.h"
#include "NavGridManager.h"

#include "DebugRenderer.h"
#include "Bullet/PhysicWorld.h"

namespace Zephyrus::AI
{
	struct NavGridManager::Impl
	{
		explicit NavGridManager::Impl(ISceneContext* context)
			: mContext(context)
		{
		}

		std::vector<ActorComponent::NavGridVolumeComponent*> mVolumeComponents;
		std::vector<GridNode> mGrid;
		Vector3D StoredNodeSize;

		float mMaxSlopeDeg = 35.1f;

		unsigned mNumPointsX = 0;
		unsigned mNumPointsY = 0;
		Vector3D mGridOrigin;

		std::vector<Debug::Debug2DArrow> mDebugArrows;
		std::vector<Debug::DebugBox> mDebugNodePosition;
		std::vector<Debug::DebugBox> mDebugAgentCollision;

		bool mPreviousShowArrow = true;
		bool mPreviousShowNodePosition = true;
		bool mPreviousShowAgentCollision = true;

		ISceneContext* mContext;

		int mNodeEdgesDebugArrowIndex = 1;
		int mNodePositionDebugBoxIndex = 1;
		int mAgentSizeDebugBoxIndex = 2;


		std::vector<GridNode*> mOpenList;

		void _SetLineTraceVisibility(Render::DebugRenderer& debugRenderer, bool visibility);
		void _SetNodePositionVisibility(Render::DebugRenderer& debugRenderer, bool visibility);
		void _SetAgentCollisionVisibility(Render::DebugRenderer& debugRenderer, bool visibility);

		void _CheckForNeighbors(Render::DebugRenderer& debugRenderer, float cellSizeX, float cellSizeY, GridNode& node);

		std::vector<GridNode*> RetraceShortestPath(GridNode* End);
	};

	NavGridManager::NavGridManager(ISceneContext* context)
		: mImpl(std::make_unique<Impl>(context))
	{
	}

	NavGridManager::~NavGridManager()
	{
		mImpl->mContext = nullptr;
		mImpl->mVolumeComponents.clear();
		mImpl->mGrid.clear();
	}

	void NavGridManager::AddVolumeComponent(ActorComponent::NavGridVolumeComponent* pComponent)
	{
		if (std::ranges::find(mImpl->mVolumeComponents.begin(), mImpl->mVolumeComponents.end(), pComponent) == mImpl->mVolumeComponents.end())
		{
			mImpl->mVolumeComponents.emplace_back(pComponent);
		}
	}
	void NavGridManager::RemoveVolumeComponent(ActorComponent::NavGridVolumeComponent* pComponent)
	{
		std::erase(mImpl->mVolumeComponents, pComponent);
	}
	void NavGridManager::ComputeGrid()
	{
		ZP_EDITOR_INFO("Computing the grid");
		mImpl->mDebugArrows.clear();
		mImpl->mDebugNodePosition.clear();
		mImpl->mDebugAgentCollision.clear();
		// work around for now it is to let the physic world know if I moved a cube i.e.
		mImpl->mContext->GetPhysicsWorld()->Update(0.01f);
		auto navVolume = mImpl->mVolumeComponents[0];
		mImpl->mPreviousShowArrow = navVolume->GetShowLines();
		mImpl->mPreviousShowAgentCollision = navVolume->GetShowAgentCollision();
		mImpl->mPreviousShowNodePosition = navVolume->GetShowNodePosition();

		mImpl->StoredNodeSize = Vector3D(navVolume->GetGridSize().x / navVolume->GetNumberOfPoints().x, navVolume->GetGridSize().y / navVolume->GetNumberOfPoints().y, navVolume->GetGridSize().z);
		
		mImpl->mNumPointsX = static_cast<int>(navVolume->GetNumberOfPoints().x);
		mImpl->mNumPointsY = static_cast<int>(navVolume->GetNumberOfPoints().y);
		mImpl->mGridOrigin = navVolume->GetWorldPosition() - navVolume->GetGridSize();
		mImpl->mGridOrigin.z = navVolume->GetWorldPosition().z;

		mImpl->mGrid.clear();
		mImpl->mGrid.reserve(mImpl->mNumPointsX * mImpl->mNumPointsY);

		auto debugRenderer = mImpl->mContext->GetRenderer()->GetDebugRenderer();

		debugRenderer->FlushDebugElements();

		for (int y = 0; y < mImpl->mNumPointsY; ++y) 
		{
			for (int x = 0; x < mImpl->mNumPointsX; ++x) 
			{
				// je fais un big line trace ca me retourne tous les hits
				// je get la normale du hit, si elle est trop horizontale ca veut dire que mon agent pourra pas passer
				// ensuite je fais un box trace dans la direction de la normale de mon hit et d'une longueur de la taille de mon agent
				// Si jamais je ne hit rien c'est que ya rien et je peux ajouter à ma map la node
				
				// une fois toutes les nodes créés, je les parcours toutes et je détermine lesquelles sont accessibles depuis lesquelles 
				// (je regarde la direction entre les 2 et si la pente (h/l) est plus grande qu'un certain nombre je dis qu'il peut pas passer par là)
				// je pense que la slope max sera de 45 degrés
				// la question qui se pose c'est comment faire si ya une rampe pour qu'il doive etre en face pour monter et pas sur les cotés 
				// (il sera bloqué sur les cotés et ca causerait un bug)
				
				Vector2D posXY = Vector2D(mImpl->mGridOrigin.x, mImpl->mGridOrigin.y);
				posXY.x += (x * mImpl->StoredNodeSize.x * 2) + mImpl->StoredNodeSize.x;
				posXY.y += (y * mImpl->StoredNodeSize.y * 2) + mImpl->StoredNodeSize.y;

				HitResult hit;
				const Vector3D startPos = Vector3D(posXY.x, posXY.y, navVolume->GetWorldPosition().z + mImpl->StoredNodeSize.z);
				const Vector3D endPos = Vector3D(posXY.x, posXY.y, navVolume->GetWorldPosition().z - mImpl->StoredNodeSize.z);
				mImpl->mContext->GetPhysicsWorld()->LineTrace(startPos, endPos, hit);

				if (hit.Normal.z < 0.7)
				{
					GridNode node = GridNode();
					node.isWalkable = false;
					Vector3D nodePos = mImpl->mGridOrigin;
					nodePos.x += (x * mImpl->StoredNodeSize.x * 2) + mImpl->StoredNodeSize.x;
					nodePos.y += (y * mImpl->StoredNodeSize.y * 2) + mImpl->StoredNodeSize.y;
					nodePos.z = hit.HitPoint.z;
					node.nodePosition = nodePos;
					node.gridX = x;
					node.gridY = y;
					mImpl->mGrid.emplace_back(node);
					continue;
				}

				HitResult hit2;
				Vector3D startPos2 = Vector3D(hit.HitPoint.x, hit.HitPoint.y, hit.HitPoint.z + navVolume->GetAgentHeight() + 0.4f);
				const Vector3D extents2 = Vector3D(navVolume->GetAgentWidth(), navVolume->GetAgentWidth(), navVolume->GetAgentHeight());
				const std::vector<Actor*> actorsToIgnore = { hit.HitActor };

				if (mImpl->mContext->GetPhysicsWorld()->BoxOverlap(startPos2, extents2, hit2, actorsToIgnore))
				{
					GridNode node = GridNode();
					node.isWalkable = false;
					Vector3D nodePos = mImpl->mGridOrigin;
					nodePos.x += (x * mImpl->StoredNodeSize.x * 2) + mImpl->StoredNodeSize.x;
					nodePos.y += (y * mImpl->StoredNodeSize.y * 2) + mImpl->StoredNodeSize.y;
					nodePos.z = hit.HitPoint.z;
					node.nodePosition = nodePos;
					node.gridX = x;
					node.gridY = y;
					mImpl->mGrid.emplace_back(node);

					Debug::DebugBox box1 = Debug::DebugBox(startPos2, extents2, hit2, Vector3D(1.0f, 1.0f, 0.0f));
					if (navVolume->GetShowAgentCollision())
					{
						mImpl->mContext->GetRenderer()->GetDebugRenderer()->AddDebugBox(box1, mImpl->mAgentSizeDebugBoxIndex);
					}
					mImpl->mDebugAgentCollision.push_back(box1);

					continue;
				}

				GridNode node = GridNode();
				node.isWalkable = true;
				Vector3D nodePos = mImpl->mGridOrigin;
				nodePos.x += (x * mImpl->StoredNodeSize.x * 2) + mImpl->StoredNodeSize.x;
				nodePos.y += (y * mImpl->StoredNodeSize.y * 2) + mImpl->StoredNodeSize.y;
				nodePos.z = hit.HitPoint.z;
				node.nodePosition = nodePos;
				node.gridX = x;
				node.gridY = y;

				Debug::DebugBox box = Debug::DebugBox(node.nodePosition, 0.1f, hit, Vector3D(0.0f, 0.0f, 1.0f));
				if (navVolume->GetShowNodePosition())
				{
					debugRenderer->AddDebugBox(box, mImpl->mNodePositionDebugBoxIndex);
				}
				mImpl->mDebugNodePosition.push_back(box);
				mImpl->mGrid.emplace_back(node);
			}
		}

		for (auto& node : mImpl->mGrid)
		{
			if (!node.isWalkable)
				continue;
			mImpl->_CheckForNeighbors(*debugRenderer, mImpl->StoredNodeSize.x, mImpl->StoredNodeSize.y, node);
		}
	}

	GridNode* NavGridManager::GetNearestNodeFromWorldPosition(const Vector3D& pWorldLocation)
	{
		auto gridRelativePosition = pWorldLocation - mImpl->mGridOrigin;
		GridNode* nearestNode = nullptr;

		int centerX = static_cast<int>(gridRelativePosition.x / (mImpl->StoredNodeSize.x * 2));
		int centerY = static_cast<int>(gridRelativePosition.y / (mImpl->StoredNodeSize.y * 2));

		centerX = zpMaths::Clamp(centerX, 0, static_cast<int>(mImpl->mNumPointsX - 1));
		centerY = zpMaths::Clamp(centerY, 0, static_cast<int>(mImpl->mNumPointsY - 1));

		int nodeIndex = centerY * mImpl->mNumPointsX + centerX;
		nearestNode = &mImpl->mGrid[nodeIndex];

		GridNode* startNode = &mImpl->mGrid[centerY * mImpl->mNumPointsX + centerX];
		if (startNode && startNode->isWalkable) {
			return startNode;
		}

		int maxRadius = std::max(mImpl->mNumPointsX, mImpl->mNumPointsY);

		for (int radius = 1; radius < maxRadius; ++radius) {
			for (int y = -radius; y <= radius; ++y) {
				for (int x = -radius; x <= radius; ++x) {

					if (std::abs(x) != radius && std::abs(y) != radius) continue;

					int testX = centerX + x;
					int testY = centerY + y;

					if (testX >= 0 && testX < (int)mImpl->mNumPointsX &&
						testY >= 0 && testY < (int)mImpl->mNumPointsY) {

						GridNode* testNode = &mImpl->mGrid[testY * mImpl->mNumPointsX + testX];
						if (testNode && testNode->isWalkable) {
							return testNode;
						}
					}
				}
			}
			if (radius > 10) break;
		}

		return nullptr;
	}

	std::vector<GridNode*> NavGridManager::GetShortestPath(GridNode* startNode, GridNode* targetNode)
	{
		if (!startNode || !targetNode) 
			return {};

		for (auto& node : mImpl->mGrid) 
		{
			node.g = zpMaths::INFINITY_POS;
			node.f = zpMaths::INFINITY_POS;
			node.parent = nullptr;
			node.isClosed = false;
		}

		mImpl->mOpenList.clear();

		startNode->g = 0;
		startNode->h = startNode->GetDistance(targetNode);
		startNode->f = startNode->h;

		mImpl->mOpenList.push_back(startNode);

		GridNode* closestNode = startNode;
		float minH = startNode->h;

		while (!mImpl->mOpenList.empty()) 
		{
			// search for the smallest f
			int indexTemp = 0;
			for (int i = 1; i < mImpl->mOpenList.size(); i++) {
				if (mImpl->mOpenList[i]->f < mImpl->mOpenList[indexTemp]->f) 
				{
					indexTemp = i;
				}
			}

			GridNode* currentNode = mImpl->mOpenList[indexTemp];

			// save the current node if it is the closest to the target point for now
			if (currentNode->h < minH) {
				minH = currentNode->h;
				closestNode = currentNode;
			}

			// if the current node is also the target we stop the search
			if (currentNode == targetNode) 
			{
				return mImpl->RetraceShortestPath(currentNode);
			}

			// the current node can no longer be in the open list
			mImpl->mOpenList.erase(mImpl->mOpenList.begin() + indexTemp);
			currentNode->isClosed = true;

			// check the neighbors and calculate their ghf
			for (GridNode* neighbor : currentNode->neighbors) 
			{
				// already processed
				if (neighbor->isClosed)
					continue;

				float tentativeG = currentNode->g + (currentNode->GetDistance(neighbor) * currentNode->weight);

				// the new g is lower than the one we had
				if (tentativeG < neighbor->g) 
				{
					neighbor->parent = currentNode;
					neighbor->g = tentativeG;
					neighbor->h = neighbor->GetDistance(targetNode);
					neighbor->f = neighbor->g + neighbor->h;

					// first time that we visit this node
					if (std::find(mImpl->mOpenList.begin(), mImpl->mOpenList.end(), neighbor) == mImpl->mOpenList.end()) 
					{
						mImpl->mOpenList.push_back(neighbor);
					}
				}
			}
		}

		// target node unreachable, return the path till the closest node found
		ZP_EDITOR_WARN("Target unreachable. Pathfinding to closest possible node.");
		return mImpl->RetraceShortestPath(closestNode);
	}
	
	void NavGridManager::UpdateDebug()
	{
		if (mImpl->mGrid.empty())
			return;
		
		auto debugRenderer = mImpl->mContext->GetRenderer()->GetDebugRenderer();
		auto navVolume = mImpl->mVolumeComponents[0];

		mImpl->_SetLineTraceVisibility(*debugRenderer, navVolume->GetShowLines());
		mImpl->_SetNodePositionVisibility(*debugRenderer, navVolume->GetShowNodePosition());
		mImpl->_SetAgentCollisionVisibility(*debugRenderer, navVolume->GetShowAgentCollision());
	}

	void NavGridManager::Impl::_SetLineTraceVisibility(Render::DebugRenderer& debugRenderer, bool visibility)
	{
		if (!visibility && mPreviousShowArrow)
		{
			debugRenderer.FlushDebugLines(mNodeEdgesDebugArrowIndex);
			mPreviousShowArrow = false;
		}
		if (visibility && !mPreviousShowArrow && !mDebugArrows.empty())
		{
			for (const auto& arrow : mDebugArrows)
			{
				debugRenderer.AddDebugArrow(arrow, mNodeEdgesDebugArrowIndex);
			}
			mPreviousShowArrow = true;
		}
	}

	void NavGridManager::Impl::_SetNodePositionVisibility(Render::DebugRenderer& debugRenderer, bool visibility)
	{
		if (!visibility && mPreviousShowNodePosition)
		{
			debugRenderer.FlushDebugBoxes(mNodeEdgesDebugArrowIndex);
			mPreviousShowNodePosition = false;
		}

		if (visibility && !mPreviousShowNodePosition && !mDebugNodePosition.empty())
		{
			for (const auto& box : mDebugNodePosition)
			{
				debugRenderer.AddDebugBox(box, mNodeEdgesDebugArrowIndex);
			}
			mPreviousShowNodePosition = true;
		}
	}

	void NavGridManager::Impl::_SetAgentCollisionVisibility(Render::DebugRenderer& debugRenderer, bool visibility)
	{
		if (!visibility && mPreviousShowAgentCollision)
		{
			debugRenderer.FlushDebugBoxes(mAgentSizeDebugBoxIndex);
			mPreviousShowAgentCollision = false;
		}

		if (visibility && !mPreviousShowAgentCollision && !mDebugAgentCollision.empty())
		{
			for (const auto& box : mDebugAgentCollision)
			{
				debugRenderer.AddDebugBox(box, mAgentSizeDebugBoxIndex);
			}
			mPreviousShowAgentCollision = true;
		}
	}
	void NavGridManager::Impl::_CheckForNeighbors(Render::DebugRenderer& debugRenderer, float cellSizeX, float cellSizeY, GridNode& node)
	{
		const int dirs[8][2] =
		{
			{1, 0}, {0, 1}, {-1, 0}, {0, -1},
			{1, 1}, {-1, -1}, {1, -1}, {-1, 1},
		};
		for (auto dir : dirs)
		{
			int nx = node.gridX + dir[0];
			int ny = node.gridY + dir[1];

			if (nx >= 0 && nx < mNumPointsX && ny >= 0 && ny < mNumPointsY) {
				int neighborIndex = ny * mNumPointsX + nx;
				GridNode& neighbor = mGrid[neighborIndex];

				if (!neighbor.isWalkable)
					continue;

				Vector3D direction = node.nodePosition - neighbor.nodePosition;

				Vector3D upDirection = Vector3D::unitZ;

				float riseMagnitude = Vector3D::Dot(direction, upDirection);

				Vector3D riseVector = riseMagnitude * upDirection;

				float runMagnitude = (direction - riseVector).Length();

				float angleDegrees = zpMaths::ToDeg(zpMaths::ATan2(riseMagnitude, runMagnitude));

				if (zpMaths::Abs(angleDegrees) >= mMaxSlopeDeg)
					continue;

				HitResult hit;
				const float height = node.nodePosition.z + 0.2f;
				const Vector3D startHit = Vector3D(node.nodePosition.x, node.nodePosition.y, height);
				const Vector3D endHit = Vector3D(neighbor.nodePosition.x * 0.9f, neighbor.nodePosition.y * 0.9f, height);

				mContext->GetPhysicsWorld()->LineTrace(startHit, endHit, hit);

				if (hit.HasHit && zpMaths::Abs(hit.Normal.z) < 0.7)
					continue;

				node.neighbors.push_back(&neighbor);

				const Vector3D startPos = Vector3D(node.nodePosition.x, node.nodePosition.y, node.nodePosition.z + 0.1f);
				const Vector3D endPos = Vector3D(neighbor.nodePosition.x, neighbor.nodePosition.y, neighbor.nodePosition.z + 0.1f);
				Debug::Debug2DArrow arrow = Debug::Debug2DArrow(startPos, endPos, Vector3D(1.0f, 0.7f, 0.0f));
				if (mVolumeComponents[0]->GetShowLines())
				{
					debugRenderer.AddDebugArrow(arrow, mNodeEdgesDebugArrowIndex);
					mDebugArrows.push_back(arrow);
				}
			}
		}
	}
	std::vector<GridNode*> NavGridManager::Impl::RetraceShortestPath(GridNode* End)
	{
		std::vector<GridNode*> pathNode;
		GridNode* temp = End;
		while (temp != nullptr)
		{
			pathNode.push_back(temp);
			temp = temp->parent;
		}
		std::reverse(pathNode.begin(), pathNode.end());
		return pathNode;
	}
}
