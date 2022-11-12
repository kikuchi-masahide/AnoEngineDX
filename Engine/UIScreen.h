#pragma once

class Scene;

class UIScreen
{
public:
	UIScreen(Scene* _scene, bool _inputflag, bool _updateflag);
	virtual void Update() = 0;
	virtual void Output() = 0;
	void SetDeleteFlag();
	bool GetDeleteFlag();
	virtual ~UIScreen() {};
	/// <summary>
	/// Inputを奥に流すかどうかを返す
	/// </summary>
	bool DoesInputGoThrough();
	/// <summary>
	/// ComponentをUpdateするかどうか返す
	/// </summary>
	bool DoesUpdateComp();
	/// <summary>
	/// Inputを奥に流すかどうかを変更する
	/// </summary>
	void SetInputTransparency(bool _flag);
	/// <summary>
	/// CompをUpdateするか否かを変更する
	/// </summary>
	void SetCompUpdateFlag(bool _flag);
	Scene* const mScene;
protected:
private:
	bool mDeleteFlag;
	//Inputを奥に流すかどうか
	bool mDoesInputGoThrough;
	//ComponentをUpdateするか
	bool mDoesUpdateComp;
};