#pragma once
#include "SourceFile/graphic/GPUBuffer/IndexBuffer.h"
#include "SourceFile/graphic/GPUBuffer/VertexBuffer.h"
#include "SourceFile/graphic/RenderContext.h"
#include "SourceFile/Noncopyable.h"

namespace Engine {
	//プリミティブ
	class Primitive:Engine::Noncopyable
	{
	public:
		//コンストラクタ
		Primitive();
		//デストラクタ
		~Primitive();

		//開放処理
		void Release();

		bool Create(
			D3D_PRIMITIVE_TOPOLOGY topology,
			int numVertex,
			int vertexStride,
			void*pSrcVertexBuffer,
			int numIndex,
			IndexBuffer::EnIndexType indexType,
			void*pSrcIndexBuffer
		);

		//描画
		//void Draw(RenderContext& rc);

		//描画
		//void Draw(RenderContext& rc, int numVertex);

		//描画
		void Draw(ID3D11DeviceContext&rc);
		//描画
		void Draw(ID3D11DeviceContext&rc, int numVertex);
	private:
		IndexBuffer m_indexBuffer;		//インデックスバッファ
		VertexBuffer m_vertexBuffer;		//頂点バッファ
		D3D_PRIMITIVE_TOPOLOGY m_topology;		//トポロジー
	};


}