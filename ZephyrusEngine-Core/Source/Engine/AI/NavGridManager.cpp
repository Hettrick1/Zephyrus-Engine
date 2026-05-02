#include "pch.h"
#include "NavGridManager.h"

#include "DebugRenderer.h"
#include "Bullet/PhysicWorld.h"

namespace Zephyrus::AI
{
	struct GridNode
	{
		GridNode() = default;
		~GridNode() = default;
		Vector3D nodePosition = Vector3D::zero;
		bool isWalkable = true;

		std::vector<GridNode*> neighbors;

		float gCost = 0.0f;
		float hCost = 0.0f;
		GridNode* parent = nullptr;
	};

	struct GridCoord
	{
		GridCoord(int x, int y) : x(x), y(y) {}
		~GridCoord() = default;
		int x;
		int y;

		friend auto operator<=>(const GridCoord& lhs, const GridCoord& rhs) = default;
	};

	struct NavGridManager::Impl
	{
		explicit NavGridManager::Impl(ISceneContext* context)
			: mContext(context)
		{
		}

		std::vector<ActorComponent::NavGridVolumeComponent*> mVolumeComponents;
		std::map<GridCoord, std::vector<GridNode>> mGrid;
		Vector3D StoredNodeSize;

		std::vector<Debug::DebugLine> mDebugLines;
		std::vector<Debug::DebugBox> mDebugNodePosition;
		std::vector<Debug::DebugBox> mDebugAgentCollision;

		bool mPreviousShowLines = true;
		bool mPreviousShowNodePosition = true;
		bool mPreviousShowAgentCollision = true;

		ISceneContext* mContext;

		void _SetLineTraceVisibility(Render::DebugRenderer& debugRenderer, bool visibility);
		void _SetNodePositionVisibility(Render::DebugRenderer& debugRenderer, bool visibility);
		void _SetAgentCollisionVisibility(Render::DebugRenderer& debugRenderer, bool visibility);
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
		mImpl->mGrid.clear();
		mImpl->mDebugLines.clear();
		mImpl->mDebugNodePosition.clear();
		mImpl->mDebugAgentCollision.clear();
		// work around for now it is to let the physic world know if I moved a cube i.e.
		mImpl->mContext->GetPhysicsWorld()->Update(0.01f);
		auto navVolume = mImpl->mVolumeComponents[0];
		mImpl->StoredNodeSize = Vector3D(navVolume->GetGridSize().x / navVolume->GetNumberOfPoints().x, navVolume->GetGridSize().y / navVolume->GetNumberOfPoints().y, navVolume->GetGridSize().z);
		
		int gridNumberOfPointsX = static_cast<int>(navVolume->GetNumberOfPoints().x * 0.5f);
		int gridNumberOfPointsY = static_cast<int>(navVolume->GetNumberOfPoints().y * 0.5f);

		auto debugRenderer = mImpl->mContext->GetRenderer()->GetDebugRenderer();

		debugRenderer->FlushDebugElements();

		for (int x = -gridNumberOfPointsX; x < gridNumberOfPointsX; ++x)
		{
			for (int y = -gridNumberOfPointsY; y < gridNumberOfPointsY; ++y)
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
				
				Vector2D posXY = Vector2D(navVolume->GetWorldPosition().x + mImpl->StoredNodeSize.x + (x * 2 * mImpl->StoredNodeSize.x), 
					navVolume->GetWorldPosition().y + mImpl->StoredNodeSize.y + (y * 2 * mImpl->StoredNodeSize.y));

				HitResult hit;
				const Vector3D startPos = Vector3D(posXY.x, posXY.y, navVolume->GetWorldPosition().z + mImpl->StoredNodeSize.z);
				const Vector3D endPos = Vector3D(posXY.x, posXY.y, navVolume->GetWorldPosition().z - mImpl->StoredNodeSize.z);
				mImpl->mContext->GetPhysicsWorld()->LineTrace(startPos, endPos, hit);

				if (hit.Normal.z < 0.7)
					continue;

				Debug::DebugLine line = Debug::DebugLine(startPos, endPos, hit);
				if (navVolume->GetShowLines())
				{
					mImpl->mContext->GetRenderer()->GetDebugRenderer()->AddDebugLine(line);
				}
				mImpl->mDebugLines.push_back(line);

				HitResult hit2;
				const Vector3D startPos2 = Vector3D(hit.HitPoint.x, hit.HitPoint.y, hit.HitPoint.z + navVolume->GetAgentHeight() * 0.5);
				const Vector3D endPos2 = startPos;
				const Vector3D extents2 = Vector3D(navVolume->GetAgentWidth(), navVolume->GetAgentWidth(), navVolume->GetAgentHeight());
				const std::vector<Actor*> actorsToIgnore = { hit.HitActor };

				if (mImpl->mContext->GetPhysicsWorld()->BoxTrace(startPos2, endPos2, extents2, hit2, actorsToIgnore))
					continue;
		
				Debug::DebugBox box1 = Debug::DebugBox(startPos2, extents2, hit2);
				if (navVolume->GetShowAgentCollision())
				{
					mImpl->mContext->GetRenderer()->GetDebugRenderer()->AddDebugBox(box1);
				}
				mImpl->mDebugAgentCollision.push_back(box1);

				GridNode node = GridNode();
				node.isWalkable = true;
				node.nodePosition = Vector3D(posXY.x, posXY.y, hit.HitPoint.z);
				GridCoord coord = GridCoord(node.nodePosition.x, node.nodePosition.y);
				Debug::DebugBox box = Debug::DebugBox(node.nodePosition, 0.1f, hit);
				if (navVolume->GetShowNodePosition())
				{
					debugRenderer->AddDebugBox(box);
				}
				mImpl->mDebugNodePosition.push_back(box);
				mImpl->mGrid[coord].push_back(node);
			}
		}
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
		if (!visibility && mPreviousShowLines)
		{
			debugRenderer.FlushDebugLines();
			mPreviousShowLines = false;
		}
		if (visibility && !mPreviousShowLines && !mDebugLines.empty())
		{
			for (const auto& line : mDebugLines)
			{
				debugRenderer.AddDebugLine(line);
			}
			mPreviousShowLines = true;
		}
	}

	void NavGridManager::Impl::_SetNodePositionVisibility(Render::DebugRenderer& debugRenderer, bool visibility)
	{
		if (!visibility && mPreviousShowNodePosition)
		{
			debugRenderer.FlushDebugBoxes();
			mPreviousShowNodePosition = false;
			if (mPreviousShowAgentCollision)
			{
				for (const auto& box : mDebugAgentCollision)
				{
					debugRenderer.AddDebugBox(box);
				}
				mPreviousShowAgentCollision = true;
			}
		}

		if (visibility && !mPreviousShowNodePosition && !mDebugNodePosition.empty())
		{
			for (const auto& box : mDebugNodePosition)
			{
				debugRenderer.AddDebugBox(box);
			}
			mPreviousShowNodePosition = true;
		}
	}

	void NavGridManager::Impl::_SetAgentCollisionVisibility(Render::DebugRenderer& debugRenderer, bool visibility)
	{
		if (!visibility && mPreviousShowAgentCollision)
		{
			debugRenderer.FlushDebugBoxes();
			mPreviousShowAgentCollision = false;
			if (mPreviousShowNodePosition)
			{
				for (const auto& box : mDebugNodePosition)
				{
					debugRenderer.AddDebugBox(box);
				}
				mPreviousShowNodePosition = true;
			}
		}

		if (visibility && !mPreviousShowAgentCollision && !mDebugAgentCollision.empty())
		{
			for (const auto& box : mDebugAgentCollision)
			{
				debugRenderer.AddDebugBox(box);
			}
			mPreviousShowAgentCollision = true;
		}
	}
}
