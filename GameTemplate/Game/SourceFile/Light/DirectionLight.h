#pragma once
#include "SourceFile/GameObject/GameObjectManager.h"
const int NUM_DIRECTION_LIG = 4;

namespace Engine {
	namespace prefab{
		class DirectionLight :public IGameObject
		{
		public:

			DirectionLight();
			~DirectionLight();

			//bool Start();
			//void Update();

			void InitDirectionLight();

			////ディレクションライトの向き
			//void SetDirection(CQuaternion dir)
			//{
			//	//dir.Multiply(m_dirLight.direction);

			//	m_dirLight.direction = dir;
			//}

			//ディレクションライトの色
			void SetColor(float color)
			{
				for (int i = 0; i < 4; i++)
				{
					m_dirLight.color[i].x = color;
					m_dirLight.color[i].y = color;
					m_dirLight.color[i].z = color;
				}
			}

			//アクティブフラグの変更
			//ディレクションライトを有効にするかどうか
			//無効なら1を入れる
			//初期設定では0が入っているのですべてのモデルでライトが有効
			void SetActiveDLFlag(int i)
			{
				m_dirLight.ActiveFlag[0] = i;
			}

			//鏡面反射のフラグ
			//0なら有効
			void SetActiveRLFlag(int i)
			{
				m_dirLight.ActiveFlag[1] = i;
			}
			void Draw();

			//定数バッファの初期化
			void InitConstantBuffer();

			//引数で渡された整数値を16の倍数に切り上げ
			int Raundup16(int n)
			{
				return (((n - 1) / 16) + 1) * 16;
			}
		private:
			struct SDirectionLight 
			{
				//CVector4 direction = { 1.0f,0.0f,0.0f,0.0f };	//ライトの向き
				//CVector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };		//ライトの色
				
				CVector4 direction[NUM_DIRECTION_LIG];	//ライトの向き
				CVector4 color[NUM_DIRECTION_LIG];		//ライトの色

				CVector3 eyePos;		//視点の座標
				float specPow;			//鏡面反射の絞り
				unsigned int ActiveFlag[3];		//アクティブフラグ0の時に有効にする	
			};
			SDirectionLight m_dirLight;
			ID3D11Buffer* m_lightCb = nullptr;		//ライト用の定数バッファ
			D3D11_BUFFER_DESC bufferDesc;
		};
	}
}