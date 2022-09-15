#pragma once
#include "level3D/TklFile.h"
#include <PhysicsStaticBox.h>
#include <iterator>
#include <xutility>


namespace nsK2EngineLow {
	class MapChipRender;

	struct LevelObjectData : Noncopyable {
		Vector3 position;
		Quaternion rotation;
		Vector3 scale;
		const wchar_t* name;
		int number = 0;

		bool EqualObjectName(const wchar_t* objName)
		{
			return wcscmp(objName, name) == 0;
		}

		bool ForwardMatchName(const wchar_t* n)
		{
			auto len = wcslen(n);
			auto namelen = wcslen(name);
			if (len > namelen) {
				//名前が長い。不一致。
				return false;
			}
			return wcsncmp(n, name, len) == 0;
		}
	};

	class PhysicsStaticBox;
	class LevelRender : public Noncopyable {
	public:
		struct BoxObjectData
		{
			Vector3 position;
			Vector3 scale;
			Quaternion rotation;
			float restitution;
		};
	private:
		using MapChipRenderPtr = std::shared_ptr<MapChipRender>;
		
	public:
		
		~LevelRender();
		/*!
		 * @brief	レベルを初期化。
		 *@param[in]	levelDataFilePath		tklファイルのファイルパス。
		 *@param[in] hookFunc				オブジェクトを作成する時の処理をフックするための関数オブジェクト。
		 *   フックしないならnullptrを指定すればよい、
		 * この関数オブジェクトがfalseを返すと、オブジェクトの情報から、
		 * 静的オブジェクトのMapChipクラスのインスタンスが生成されます。
		 * オブジェクトの名前などで、ドアやジャンプ台、アイテムなどの特殊なクラスのインスタンスを生成したときに、
		 * デフォルトで作成されるMapChipクラスのインスタンスが不要な場合はtrueを返してください。
		 * 例えば、フック関数の中で、渡されたオブジェクトデータの名前のモデルを描画するクラスのインスタンスを
		 * 生成したときに、falseを返してしまうと、同じモデルが二つ描画されることになります。
		 */
		void Init(const char* filePath, std::function<bool(LevelObjectData& objData)> hookFunc);
		/// <summary>
		/// 更新処理。
		/// </summary>
		void Update();
		/// <summary>
		/// モデルを描画。
		/// </summary>
		/// <param name="rc">レンダーコンテキスト。</param>
		void Draw(RenderContext& rc);

		/// <summary>
		/// BoxColliderを生成する。
		/// 反発力は指定しなければ0.0f。
		/// </summary>
		/// <param name="scale">サイズ。</param>
		/// <param name="resitutation">反発力。</param>
		void InitBoxCollider(const LevelObjectData& objData,float const& restitution=0.0f);
		/// <summary>
		/// boxColliderを廃棄する。
		/// </summary>
		void ReleaseBoxCollider();
				
		void CreateBox(const BoxObjectData* objData);
		
	private:
		/// <summary>
		/// マップチップを作成。
		/// </summary>
		/// <param name="objData">LevelObjectData。</param>
		void CreateMapChipRender(const LevelObjectData& objData, const char* filePath);
		/// <summary>
		/// Tklファイルの行列を変換する。
		/// </summary>
		void MatrixTklToLevel();


	private:
		using BonePtr = std::unique_ptr<Bone>;								//ボーンPtr。
		std::vector<BonePtr> m_bonelist;									//ボーンのリスト。
		std::unique_ptr<Matrix[]> m_matrixlist;								//行列のリスト。
		std::map<std::string, MapChipRenderPtr> m_mapChipRenderPtrs;		//マップチップの可変長配列。
		TklFile m_tklFile;													//Tklファイル。
		std::unique_ptr<BoxObjectData>	m_boxobject;
		std::vector<std::unique_ptr<PhysicsStaticBox>> m_collider;

		};

}