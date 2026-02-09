#include "pch.h"
#include "TheDescentPlayer.h"

#include "pch.h"
#include "Log.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Scene.h"

using Zephyrus::Assets::AssetsManager;

namespace Zephyrus::ActorComponent
{
	TheDescentPlayer::TheDescentPlayer(Actor* pOwner, int updateOder)
		: Component(pOwner, "TheDescentPlayer", updateOder)
	{
	}

	TheDescentPlayer::~TheDescentPlayer()
	{
	}

	void TheDescentPlayer::Deserialize(Serialization::IDeserializer& pReader)
	{
		Component::Deserialize(pReader);
	}

	void TheDescentPlayer::Serialize(Serialization::ISerializer& pWriter)
	{
		Component::Serialize(pWriter);
	}

	void TheDescentPlayer::OnStart()
	{
		Component::OnStart();
	}

	void TheDescentPlayer::Update()
	{
		Component::Update();
	}

	void TheDescentPlayer::OnEnd()
	{
		Component::OnEnd();
	}
}