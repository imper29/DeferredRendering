#pragma once
template<typename TEvent>
class SceneSystemListener
{
public:
	virtual void OnEvtReceived(TEvent& evt) = 0;
};
