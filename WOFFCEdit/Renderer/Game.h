//
// Game.h
//
#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "../Tool/SceneObject.h"
#include "DisplayObject.h"
#include "DisplayChunk.h"
#include "../Tool/ChunkObject.h"
#include "../Tool/InputCommands.h"
#include <vector>

#include "../Tool/Camera.h"

//A basic game implementation that creates a D3D11 device and provides a game loop
class Game : public DX::IDeviceNotify
{
public:
	Game();
	~Game();

	//Initialization and management
	void Initialize(HWND window, int width, int height);
	void SetGridState(bool state);

	//Basic game loop
	void Tick(const InputCommands* input);
	void Render();

	//Rendering helpers
	void Clear();

	//IDeviceNotify
	virtual void OnDeviceLost() override;
	virtual void OnDeviceRestored() override;

	//Messages
	void OnActivated();
	void OnDeactivated();
	void OnSuspending();
	void OnResuming();
	void OnWindowSizeChanged(int width, int height);

	//Tool-specific
	void BuildDisplayList(const std::vector<SceneObject>* sceneGraph);
	void BuildDisplayChunk(const ChunkObject* sceneChunk);
	void SaveDisplayChunk(ChunkObject* sceneChunk);
	void ClearDisplayList();

	//Functionality
	int MousePicking() const;
	void Delete(int& selectedID);
	void Copy(int selectedID);
	void Cut(int& selectedID);
	void Paste();

#ifdef DXTK_AUDIO
	void NewAudioDevice();
#endif

private:
	void Update(DX::StepTimer const& timer);

	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();

	void XM_CALLCONV DrawGrid(DirectX::FXMVECTOR xAxis, DirectX::FXMVECTOR yAxis, DirectX::FXMVECTOR origin, size_t xDivs, size_t yDivs, DirectX::GXMVECTOR color);

	//Tool-specific
	std::vector<DisplayObject>		m_displayList{};
	DisplayChunk					m_displayChunk;
	InputCommands					m_inputCommands{};

	//Screen size
	RECT							m_screenDimensions{};
	
	//Camera
	std::unique_ptr<Camera>			m_camera;

	//Copy/paste
	DisplayObject					m_objectToCopy;

	//Control variables
	//Grid rendering on/off
	bool m_grid;					
	//Device resources
    std::shared_ptr<DX::DeviceResources>    m_deviceResources;

    //Rendering loop timer
    DX::StepTimer                           m_timer;

    //Input devices
    std::unique_ptr<DirectX::GamePad>       m_gamePad{};
    std::unique_ptr<DirectX::Keyboard>      m_keyboard{};
    std::unique_ptr<DirectX::Mouse>         m_mouse{};

    //DirectXTK objects.
    std::unique_ptr<DirectX::CommonStates>                                  m_states{};
    std::unique_ptr<DirectX::BasicEffect>                                   m_batchEffect{};
    std::unique_ptr<DirectX::EffectFactory>                                 m_fxFactory{};
    std::unique_ptr<DirectX::GeometricPrimitive>                            m_shape{};
    std::unique_ptr<DirectX::Model>                                         m_model{};
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>  m_batch{};
    std::unique_ptr<DirectX::SpriteBatch>                                   m_sprites{};
    std::unique_ptr<DirectX::SpriteFont>                                    m_font{};

#ifdef DXTK_AUDIO
    std::unique_ptr<DirectX::AudioEngine>                                   m_audEngine;
    std::unique_ptr<DirectX::WaveBank>                                      m_waveBank;
    std::unique_ptr<DirectX::SoundEffect>                                   m_soundEffect;
    std::unique_ptr<DirectX::SoundEffectInstance>                           m_effect1;
    std::unique_ptr<DirectX::SoundEffectInstance>                           m_effect2;
#endif

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_texture1;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>                        m_texture2;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>                               m_batchInputLayout;

#ifdef DXTK_AUDIO
    uint32_t                                                                m_audioEvent;
    float                                                                   m_audioTimerAcc;

    bool                                                                    m_retryDefault;
#endif

    DirectX::SimpleMath::Matrix                                             m_world;
    DirectX::SimpleMath::Matrix                                             m_view;
    DirectX::SimpleMath::Matrix                                             m_projection;
};

std::wstring StringToWCHART(std::string s);