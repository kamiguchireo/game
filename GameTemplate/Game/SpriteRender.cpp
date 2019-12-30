#include "stdafx.h"
#include "SpriteRender.h"

namespace Engine {
	namespace prefab {
		//初期化処理
		void SpriteRender::Init(const wchar_t*texFilePath, float w, float h, bool isDraw3D)
		{
			m_isDraw3D = isDraw3D;
			m_texture.CreateFromDDSTextureFromFile(texFilePath);
			m_sprite.Init(m_texture, w, h);
		}

		//更新処理
		void SpriteRender::Update()
		{
			//スプライトにテクスチャを設定する
			m_sprite.SetTexture(m_texture);
			//スプライトの更新処理を呼び出す
			m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);
		}

	}
}