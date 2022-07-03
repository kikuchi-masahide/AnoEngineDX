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
	/// Input‚ğ‰œ‚É—¬‚·‚©‚Ç‚¤‚©‚ğ•Ô‚·
	/// </summary>
	bool DoesInputGoThrough();
	/// <summary>
	/// Component‚ğUpdate‚·‚é‚©‚Ç‚¤‚©•Ô‚·
	/// </summary>
	bool DoesUpdateComp();
	/// <summary>
	/// Input‚ğ‰œ‚É—¬‚·‚©‚Ç‚¤‚©‚ğ•ÏX‚·‚é
	/// </summary>
	void SetInputTransparency(bool _flag);
	/// <summary>
	/// Comp‚ğUpdate‚·‚é‚©”Û‚©‚ğ•ÏX‚·‚é
	/// </summary>
	void SetCompUpdateFlag(bool _flag);
	Scene* const mScene;
protected:
private:
	bool mDeleteFlag;
	//Input‚ğ‰œ‚É—¬‚·‚©‚Ç‚¤‚©
	bool mDoesInputGoThrough;
	//Component‚ğUpdate‚·‚é‚©
	bool mDoesUpdateComp;
};