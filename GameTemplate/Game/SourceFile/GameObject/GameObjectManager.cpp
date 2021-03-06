#include "stdafx.h"
#include "GameObjectManager.h"

namespace Engine {

	void CGameObjectManager::PreRender()
	{
		for (GameObjectList objList : m_gameObjectListArray)
		{
			for (IGameObject*obj : objList)
			{
				obj->PreRenderWrapper();
			}
		}
	}

	void CGameObjectManager::Start()
	{
		for (GameObjectList objList : m_gameObjectListArray)
		{
			for (IGameObject*obj : objList)
			{
				obj->StartWrapper();

			}
		}
	}

	void CGameObjectManager::Update()
	{
		for (GameObjectList objList : m_gameObjectListArray)
		{
			for (IGameObject*obj : objList)
			{
				obj->UpdateWrapper();
			}
		}
		ExecuteDeleteGameObjects();
	}

	void CGameObjectManager::Draw()
	{
		for (GameObjectList objList : m_gameObjectListArray)
		{
			for (IGameObject*obj : objList)
			{
				obj->DrawWrapper();
			}
		}
	}

	void CGameObjectManager::EffectDraw()
	{
		Effekseer::Matrix44 efCameraMat;
		g_camera3D.GetViewMatrix().CopyTo(efCameraMat);
		Effekseer::Matrix44 efProjMat;
		g_camera3D.GetProjectionMatrix().CopyTo(efProjMat);
		//カメラ行列とプロジェクション行列を設定。
		g_graphicsEngine->GetEffekseerRender().SetCameraMatrix(efCameraMat);
		g_graphicsEngine->GetEffekseerRender().SetProjectionMatrix(efProjMat);

		g_graphicsEngine->GetEffekseerManager().Update(g_gameTime.GetFrameDeltaTime() * 60.0f);

		g_graphicsEngine->GetEffekseerRender().BeginRendering();
		g_graphicsEngine->GetEffekseerManager().Draw();
		g_graphicsEngine->GetEffekseerRender().EndRendering();
	}

	void CGameObjectManager::PostRender()
	{
		for (GameObjectList objList : m_gameObjectListArray)
		{
			for (IGameObject*obj : objList)
			{
				obj->PostRenderWrapper();
			}
		}
	}
	
	void CGameObjectManager::ExecuteDeleteGameObjects()
	{
		int preBufferNo = m_currentDeleteObjectBufferNo;
		//バッファを切り替え
		m_currentDeleteObjectBufferNo = 1 ^ m_currentDeleteObjectBufferNo;
		for (GameObjectList&goList : m_deleteObjectArray[preBufferNo])
		{
			for (IGameObject*go : goList)
			{
				GameObjectPrio prio = go->GetPriority();
				GameObjectList&goExecList = m_gameObjectListArray.at(prio);
				auto it = std::find(goExecList.begin(), goExecList.end(), go);
				if (it != goExecList.end())
				{
					//削除リストから除外。
					(*it)->m_isRegistDeadList = false;
					if ((*it)->IsNewFromGameObjectManagr())
					{
						delete(*it);
					}

				}
				goExecList.erase(it);
			}
			goList.clear();
		}
	}



}