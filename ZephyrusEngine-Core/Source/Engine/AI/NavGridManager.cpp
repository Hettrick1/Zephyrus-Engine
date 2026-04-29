#include "pch.h"
#include "NavGridManager.h"

#include "DebugRenderer.h"
#include "Bullet/PhysicWorld.h"

namespace Zephyrus::AI
{
	NavGridManager::NavGridManager(ISceneContext* context)
		: mContext(context)
	{
	}

	NavGridManager::~NavGridManager()
	{
		mContext = nullptr;
		mVolumeComponents.clear();
		mGrid.clear();
	}

	void NavGridManager::AddVolumeComponent(ActorComponent::NavGridVolumeComponent* pComponent)
	{
		if (std::ranges::find(mVolumeComponents.begin(), mVolumeComponents.end(), pComponent) == mVolumeComponents.end())
		{
			mVolumeComponents.emplace_back(pComponent);
		}
	}
	void NavGridManager::RemoveVolumeComponent(ActorComponent::NavGridVolumeComponent* pComponent)
	{
		std::erase(mVolumeComponents, pComponent);
	}
	void NavGridManager::ComputeGrid()
	{
		ZP_EDITOR_INFO("Computing the grid");
		mGrid.clear();
		mDebugLines.clear();
		// work around for now it is to let the physic world know if I moved a cube i.e.
		mContext->GetPhysicsWorld()->Update(0.01f);
		auto navVolume = mVolumeComponents[0];
		StoredNodeSize = Vector3D(navVolume->GetAgentWidth(), navVolume->GetAgentWidth(), navVolume->GetAgentHeight());
		
		int gridHalfSizeX = static_cast<int>(navVolume->GetGridSize().x * 0.5f);
		int gridHalfSizeY = static_cast<int>(navVolume->GetGridSize().y * 0.5f);
		
		auto debugRenderer = mContext->GetRenderer()->GetDebugRenderer();

		debugRenderer->FlushDebugElements();

		for (int x = -gridHalfSizeX; x < gridHalfSizeX; ++x)
		{
			for (int y = -gridHalfSizeY; y < gridHalfSizeY; ++y)
			{
				// je fais un big line trace ca me retourne tous les hits
				// je get la normale du hit, si elle est trop horizontale ca veut dire que mon agent pourra pas passer
				// ensuite je fais un box trace dans la direction de la normale de mon hit et d'une longueur de la taille de mon agent
				// Si jamais je ne hit rien c'est que ya rien et je peux ajouter à ma map la node
				
				// une fois toutes les nodes créés, je les parcours toutes et je détermine lesquelles sont accessibles depuis lesquelles 
				// (je regarde la direction entre les 2 et si la pente (h/l) est plus granqu'un certain nombre je dis qu'il peut pas passer par là)
				// je pense que la slope max sera de 45 degrés
				// la question qui se pose c'est comment faire si ya une rampe pour qu'il doive etre en face pour monter et pas sur les cotés 
				// (il sera bloqué sur les cotés et ca causerait un bug)
				
				Vector2D posXY = Vector2D(navVolume->GetWorldPosition().x + StoredNodeSize.x + (x * StoredNodeSize.x * 2), navVolume->GetWorldPosition().y + StoredNodeSize.y + (y * StoredNodeSize.y * 2));

				HitResult hit;
				const Vector3D startPos = Vector3D(posXY.x, posXY.y, navVolume->GetWorldPosition().z + 10);
				const Vector3D endPos = Vector3D(posXY.x, posXY.y, navVolume->GetWorldPosition().z - 10);
				mContext->GetPhysicsWorld()->LineTrace(startPos, endPos, hit);

				if (hit.Normal.z < 0.7)
					continue;

				Debug::DebugLine line = Debug::DebugLine(startPos, endPos, hit);
				mContext->GetRenderer()->GetDebugRenderer()->AddDebugLine(line);
				mDebugLines.push_back(line);

				GridNode node = GridNode();
				node.isWalkable = true;
				node.nodePosition = Vector3D(posXY.x, posXY.y, hit.HitPoint.z);
				GridCoord coord = GridCoord(node.nodePosition.x, node.nodePosition.y);
				Debug::DebugBox box = Debug::DebugBox(node.nodePosition, 0.1f, hit);
				debugRenderer->AddDebugBox(box);
				mDebugNodePosition.push_back(box);
				mGrid[coord].push_back(node);
			}
		}
	}
	
	void NavGridManager::UpdateDebug()
	{
		if (mGrid.empty())
			return;
		
		auto debugRenderer = mContext->GetRenderer()->GetDebugRenderer();
		auto navVolume = mVolumeComponents[0];

		if (!navVolume->GetShowLines() && mPreviousShowLines)
		{
			debugRenderer->FlushDebugLines();
			mPreviousShowLines = false;
		}
		if (navVolume->GetShowLines() && !mPreviousShowLines && !mDebugLines.empty())
		{
			for (const auto& line : mDebugLines)
			{
				mContext->GetRenderer()->GetDebugRenderer()->AddDebugLine(line);
			}
			mPreviousShowLines = true;
		}

		if (!navVolume->GetShowNodePosition() && mPreviousShowNodePosition)
		{
			debugRenderer->FlushDebugBoxes();
			mPreviousShowNodePosition = false;
		}

		if (navVolume->GetShowNodePosition() && !mPreviousShowNodePosition && !mDebugNodePosition.empty())
		{
			for (const auto& box : mDebugNodePosition)
			{
				debugRenderer->AddDebugBox(box);
			}
			mPreviousShowNodePosition = true;
		}
	}
}
