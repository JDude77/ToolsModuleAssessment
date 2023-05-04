#include "PasteCommand.h"
#include "../../Renderer/DeviceResources.h"

PasteCommand::PasteCommand(std::vector<DisplayObject>& displayList, const DisplayObject& objectToPaste, const std::shared_ptr<DX::DeviceResources>& deviceResources)
	: m_displayList(displayList), m_objectToPaste(objectToPaste), m_deviceResources(deviceResources)
{
    m_fxFactory = new DirectX::EffectFactory(deviceResources->GetD3DDevice());
}//End constructor

void PasteCommand::Execute()
{
	//Can't paste if we don't have anything to paste
    if(m_objectToPaste.m_model == nullptr) return;

    //Create a temporary display object that we will populate then append to the display list
	DisplayObject newDisplayObject;
	
    //Get DXSDK to load model
    //Set final boolean to "false" for left-handed coordinate system (Maya)
	newDisplayObject.m_model = DirectX::Model::CreateFromCMO(m_deviceResources->GetD3DDevice(), m_objectToPaste.m_model_path.c_str(), *m_fxFactory, true);	

    //Save the model path for completeness
	newDisplayObject.m_model_path = m_objectToPaste.m_model_path;

    //Load diffuse texture into shader resource
	const HRESULT rs = DirectX::CreateDDSTextureFromFile(m_deviceResources->GetD3DDevice(), m_objectToPaste.m_texture_diffuse_path.c_str(), nullptr, &newDisplayObject.m_texture_diffuse);	

    //Save the texture path for completeness
	newDisplayObject.m_texture_diffuse_path = m_objectToPaste.m_texture_diffuse_path;

	//If texture loading fails, load error default
	if (rs)
	{
        //Load texture into shader resource
        DirectX::CreateDDSTextureFromFile(m_deviceResources->GetD3DDevice(), L"database/data/Error.dds", nullptr, &newDisplayObject.m_texture_diffuse);
	}//End if

	//Apply new texture to model's effect
    //Handled using an in-line function via Lambda
	newDisplayObject.m_model->UpdateEffects([&](DirectX::IEffect* effect)
	{
		const auto lights = dynamic_cast<DirectX::BasicEffect*>(effect);
		if (lights)
		{
			lights->SetTexture(newDisplayObject.m_texture_diffuse);			
		}//End if
	});

    //Slightly offset the position to prevent overlapping
    newDisplayObject.m_position = DirectX::SimpleMath::Vector3
    (
		m_objectToPaste.m_position.x + 0.25f,
        m_objectToPaste.m_position.y + 0.25f,
        m_objectToPaste.m_position.z + 0.25f
    );

    //Set equivalent scale
    newDisplayObject.m_scale = m_objectToPaste.m_scale;

    //Set equivalent orientation
    newDisplayObject.m_orientation = m_objectToPaste.m_orientation;

    //Create the new object in the display list
    m_displayList.push_back(newDisplayObject);
}//End Paste Execute

void PasteCommand::Undo()
{
    //Remove the object from the back of the display list
    m_displayList.pop_back();
}//End Paste Undo