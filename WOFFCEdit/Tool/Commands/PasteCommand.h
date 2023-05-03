#pragma once
#include "Command.h"
#include <vector>
#include "../../Renderer/DisplayObject.h"

namespace DX
{
	class DeviceResources;
}

class PasteCommand : public Command
{
public:
	PasteCommand(std::vector<DisplayObject>& displayList, const DisplayObject& objectToPaste, const std::shared_ptr<DX::DeviceResources>& deviceResources);
	~PasteCommand() override = default;
	void Execute() override;
	void Undo() override;

private:
	std::vector<DisplayObject>& m_displayList;
	DisplayObject m_objectToPaste;
	std::shared_ptr<DX::DeviceResources> m_deviceResources;
	DirectX::EffectFactory* m_fxFactory{};
};
