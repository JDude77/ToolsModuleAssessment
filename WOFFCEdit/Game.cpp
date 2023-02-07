//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "DisplayObject.h"
#include <string>

using namespace DirectX;
using namespace SimpleMath;
using Microsoft::WRL::ComPtr;

Game::Game()
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
	m_displayList.clear();
	
	//Initial settings
	//Modes
	m_grid = false;

	//Functional
	m_movespeed = 0.30;
	m_camRotRate = 3.0;

	//Camera
	m_camPosition.x = 0.0f;
	m_camPosition.y = 3.7f;
	m_camPosition.z = -3.5f;

	m_camOrientation.x = 0;
	m_camOrientation.y = 0;
	m_camOrientation.z = 0;

	m_camLookAt.x = 0.0f;
	m_camLookAt.y = 0.0f;
	m_camLookAt.z = 0.0f;

	m_camLookDirection.x = 0.0f;
	m_camLookDirection.y = 0.0f;
	m_camLookDirection.z = 0.0f;

	m_camRight.x = 0.0f;
	m_camRight.y = 0.0f;
	m_camRight.z = 0.0f;

	m_camOrientation.x = 0.0f;
	m_camOrientation.y = 0.0f;
	m_camOrientation.z = 0.0f;
}//End default constructor

Game::~Game()
{
#ifdef DXTK_AUDIO
    if (m_audEngine)
    {
        m_audEngine->Suspend();
    }//End if
#endif
}//End destructor

//Initialize the Direct3D resources required to run
void Game::Initialize(HWND window, int width, int height)
{
    m_gamePad = std::make_unique<GamePad>();
    m_keyboard = std::make_unique<Keyboard>();
    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(window);

    m_deviceResources->SetWindow(window, width, height);
    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

#ifdef DXTK_AUDIO
    //Create DirectXTK for audio objects
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags = eflags | AudioEngine_Debug;
#endif

    m_audEngine = std::make_unique<AudioEngine>(eflags);

    m_audioEvent = 0;
    m_audioTimerAcc = 10.f;
    m_retryDefault = false;

    m_waveBank = std::make_unique<WaveBank>(m_audEngine.get(), L"adpcmdroid.xwb");

    m_soundEffect = std::make_unique<SoundEffect>(m_audEngine.get(), L"MusicMono_adpcm.wav");
    m_effect1 = m_soundEffect->CreateInstance();
    m_effect2 = m_waveBank->CreateInstance(10);

    m_effect1->Play(true);
    m_effect2->Play();
#endif
}//End Initialize

void Game::SetGridState(bool state)
{
	m_grid = state;
}//End SetGridState

#pragma region Frame Update
//Executes the basic game loop
void Game::Tick(InputCommands *Input)
{
	//Copy over input commands so we have a local version to use elsewhere
	m_InputCommands = *Input;
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

#ifdef DXTK_AUDIO
    // Only update audio engine once per frame
    if (!m_audEngine->IsCriticalError() && m_audEngine->Update())
    {
        // Setup a retry in 1 second
        m_audioTimerAcc = 1.f;
        m_retryDefault = true;
    }//End if
#endif

    Render();
}//End Tick

//Updates the world
void Game::Update(DX::StepTimer const& timer)
{
	//TODO: Any more complex than this, and the camera should be abstracted out to somewhere else
	//Camera motion is on a plane, so kill the 7 component of the look direction
    //^ I don't even know what the 7 component is to make that comment make more sense - y, maybe???
	Vector3 planarMotionVector = m_camLookDirection;
	planarMotionVector.y = 0.0;

    //Change camera orientation based on rotation inputs
	if (m_InputCommands.rotRight) m_camOrientation.y -= m_camRotRate;
	if (m_InputCommands.rotLeft) m_camOrientation.y += m_camRotRate;

	//Create look direction from Euler angles in m_camOrientation
	m_camLookDirection.x = sin((m_camOrientation.y)*3.1415 / 180);
	m_camLookDirection.z = cos((m_camOrientation.y)*3.1415 / 180);
	m_camLookDirection.Normalize();

	//Create right vector from look direction
	m_camLookDirection.Cross(Vector3::UnitY, m_camRight);

	//Process input and update camera, etc.
	if (m_InputCommands.forward)    m_camPosition += m_camLookDirection * m_movespeed;
	if (m_InputCommands.back)       m_camPosition -= m_camLookDirection * m_movespeed;
	if (m_InputCommands.right)		m_camPosition += m_camRight         * m_movespeed;
	if (m_InputCommands.left)		m_camPosition -= m_camRight         * m_movespeed;

	//Update look-at point
	m_camLookAt = m_camPosition + m_camLookDirection;

	//Apply camera vectors
    m_view = Matrix::CreateLookAt(m_camPosition, m_camLookAt, Vector3::UnitY);

    m_batchEffect->SetView(m_view);
    m_batchEffect->SetWorld(Matrix::Identity);
	m_displayChunk.m_terrainEffect->SetView(m_view);
	m_displayChunk.m_terrainEffect->SetWorld(Matrix::Identity);

#ifdef DXTK_AUDIO
    m_audioTimerAcc -= (float)timer.GetElapsedSeconds();
    if (m_audioTimerAcc < 0)
    {
        if (m_retryDefault)
        {
            m_retryDefault = false;
            if (m_audEngine->Reset())
            {
                // Restart looping audio
                m_effect1->Play(true);
            }//End if
        }//End if
        else
        {
            m_audioTimerAcc = 4.f;

            m_waveBank->Play(m_audioEvent++);

            if (m_audioEvent >= 11)
                m_audioEvent = 0;
        }//End else
    }//End if
#endif
}//End Update
#pragma endregion

#pragma region Frame Render
//Draws the scene
void Game::Render()
{
    //Don't try to render anything before the first update
    if (m_timer.GetFrameCount() == 0) return;

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

	if (m_grid)
	{
		//Draw procedurally-generated dynamic grid
		const XMVECTORF32 xaxis = { 512.f, 0.f, 0.f };
		const XMVECTORF32 yaxis = { 0.f, 0.f, 512.f };
		DrawGrid(xaxis, yaxis, g_XMZero, 512, 512, Colors::Gray);
	}//End if

	//CAMERA POSITION ON HUD
	m_sprites->Begin();
	WCHAR   Buffer[256];
	std::wstring cameraPositionText =
        L"Cam X: " + std::to_wstring(m_camPosition.x) +
        L"     " +
        L"Cam Z: " + std::to_wstring(m_camPosition.z);
	m_font->DrawString(m_sprites.get(), cameraPositionText.c_str() , XMFLOAT2(100, 10), Colors::Yellow);
	m_sprites->End();

	//RENDER OBJECTS FROM SCENEGRAPH
	int numRenderObjects = m_displayList.size();
	for (int i = 0; i < numRenderObjects; i++)
	{
		m_deviceResources->PIXBeginEvent(L"Draw Model");
		const XMVECTORF32 scale = { m_displayList[i].m_scale.x,
									m_displayList[i].m_scale.y,
									m_displayList[i].m_scale.z };

		const XMVECTORF32 translate = { m_displayList[i].m_position.x,
										m_displayList[i].m_position.y,
										m_displayList[i].m_position.z };

		//Convert degrees into radians for rotation matrix
		XMVECTOR rotate = Quaternion::CreateFromYawPitchRoll(m_displayList[i].m_orientation.y *3.1415 / 180,
															m_displayList[i].m_orientation.x *3.1415 / 180,
															m_displayList[i].m_orientation.z *3.1415 / 180);

		XMMATRIX local = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, translate);

		//Last variable in draw - make last boolean TRUE for wireframe mode
		m_displayList[i].m_model->Draw(context, *m_states, local, m_view, m_projection, false);

		m_deviceResources->PIXEndEvent();
	}//End for
    m_deviceResources->PIXEndEvent();

	//RENDER TERRAIN
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(m_states->DepthDefault(),0);
	context->RSSetState(m_states->CullNone());
    //Uncomment for wireframe
	//context->RSSetState(m_states->Wireframe());		

	//Render the batch
	//This is handled in the display chunk becuase it has the potential to get complex
	m_displayChunk.RenderBatch(m_deviceResources);

    m_deviceResources->Present();
}//End Render

//Helper method to clear the back buffers
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    //Clear the views
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetBackBufferRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    //Set the viewport
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}//End Clear

void XM_CALLCONV Game::DrawGrid(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color)
{
    m_deviceResources->PIXBeginEvent(L"Draw Grid");

    auto context = m_deviceResources->GetD3DDeviceContext();
    context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
    context->OMSetDepthStencilState(m_states->DepthNone(), 0);
    context->RSSetState(m_states->CullCounterClockwise());

    m_batchEffect->Apply(context);

    context->IASetInputLayout(m_batchInputLayout.Get());

    m_batch->Begin();

    xdivs = std::max<size_t>(1, xdivs);
    ydivs = std::max<size_t>(1, ydivs);

    for (size_t i = 0; i <= xdivs; ++i)
    {
        float fPercent = float(i) / float(xdivs);
        fPercent = (fPercent * 2.0f) - 1.0f;
        XMVECTOR vScale = XMVectorScale(xAxis, fPercent);
        vScale = XMVectorAdd(vScale, origin);

        VertexPositionColor v1(XMVectorSubtract(vScale, yAxis), color);
        VertexPositionColor v2(XMVectorAdd(vScale, yAxis), color);
        m_batch->DrawLine(v1, v2);
    }//End for

    for (size_t i = 0; i <= ydivs; i++)
    {
        float fPercent = float(i) / float(ydivs);
        fPercent = (fPercent * 2.0f) - 1.0f;
        XMVECTOR vScale = XMVectorScale(yAxis, fPercent);
        vScale = XMVectorAdd(vScale, origin);

        VertexPositionColor v1(XMVectorSubtract(vScale, xAxis), color);
        VertexPositionColor v2(XMVectorAdd(vScale, xAxis), color);
        m_batch->DrawLine(v1, v2);
    }//End for

    m_batch->End();

    m_deviceResources->PIXEndEvent();
}//End DrawGrid
#pragma endregion

#pragma region Message Handlers
//Message handlers
void Game::OnActivated()
{
}//End OnActivated

void Game::OnDeactivated()
{
}//End OnDeactivated

void Game::OnSuspending()
{
#ifdef DXTK_AUDIO
    m_audEngine->Suspend();
#endif
}//End OnSuspending

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

#ifdef DXTK_AUDIO
    m_audEngine->Resume();
#endif
}//End OnResuming

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();
}//End OnWindowSizeChanged

void Game::BuildDisplayList(std::vector<SceneObject> * SceneGraph)
{
	auto device = m_deviceResources->GetD3DDevice();
	auto devicecontext = m_deviceResources->GetD3DDeviceContext();

	if (!m_displayList.empty()) m_displayList.clear();

	int numObjects = SceneGraph->size();
    //For every item in the SceneGraph
	for (int i = 0; i < numObjects; i++)
	{
		//Create a temporary display object that we will populate then append to the display list
		DisplayObject newDisplayObject;
		
		//Load the model
		std::wstring modelwstr = StringToWCHART(SceneGraph->at(i).model_path);
        //Get DXSDK to load model
        //Set final boolean to "false" for left-handed coordinate system (Maya)
		newDisplayObject.m_model = Model::CreateFromCMO(device, modelwstr.c_str(), *m_fxFactory, true);	

		//Load diffuse texture
		std::wstring texturewstr = StringToWCHART(SceneGraph->at(i).tex_diffuse_path);
        //Load texture into shader resource
		HRESULT rs = CreateDDSTextureFromFile(device, texturewstr.c_str(), nullptr, &newDisplayObject.m_texture_diffuse);	

		//If texture loading fails, load error default
		if (rs)
		{
            //Load texture into shader resource
			CreateDDSTextureFromFile(device, L"database/data/Error.dds", nullptr, &newDisplayObject.m_texture_diffuse);
		}//End if

		//Apply new texture to model's effect
        //Handled using an in-line function via Lambda
		newDisplayObject.m_model->UpdateEffects([&](IEffect* effect)
		{	
			auto lights = dynamic_cast<BasicEffect*>(effect);
			if (lights)
			{
				lights->SetTexture(newDisplayObject.m_texture_diffuse);			
			}//End if
		});

		//Set position
		newDisplayObject.m_position.x = SceneGraph->at(i).posX;
		newDisplayObject.m_position.y = SceneGraph->at(i).posY;
		newDisplayObject.m_position.z = SceneGraph->at(i).posZ;
		
		//Set orientation
		newDisplayObject.m_orientation.x = SceneGraph->at(i).rotX;
		newDisplayObject.m_orientation.y = SceneGraph->at(i).rotY;
		newDisplayObject.m_orientation.z = SceneGraph->at(i).rotZ;

		//Set scale
		newDisplayObject.m_scale.x = SceneGraph->at(i).scaX;
		newDisplayObject.m_scale.y = SceneGraph->at(i).scaY;
		newDisplayObject.m_scale.z = SceneGraph->at(i).scaZ;

		//Set wireframe/render flags
		newDisplayObject.m_render		= SceneGraph->at(i).editor_render;
		newDisplayObject.m_wireframe	= SceneGraph->at(i).editor_wireframe;

        //Set light data
		newDisplayObject.m_light_type		= SceneGraph->at(i).light_type;
		newDisplayObject.m_light_diffuse_r	= SceneGraph->at(i).light_diffuse_r;
		newDisplayObject.m_light_diffuse_g	= SceneGraph->at(i).light_diffuse_g;
		newDisplayObject.m_light_diffuse_b	= SceneGraph->at(i).light_diffuse_b;
		newDisplayObject.m_light_specular_r = SceneGraph->at(i).light_specular_r;
		newDisplayObject.m_light_specular_g = SceneGraph->at(i).light_specular_g;
		newDisplayObject.m_light_specular_b = SceneGraph->at(i).light_specular_b;
		newDisplayObject.m_light_spot_cutoff = SceneGraph->at(i).light_spot_cutoff;
		newDisplayObject.m_light_constant	= SceneGraph->at(i).light_constant;
		newDisplayObject.m_light_linear		= SceneGraph->at(i).light_linear;
		newDisplayObject.m_light_quadratic	= SceneGraph->at(i).light_quadratic;
		
		m_displayList.push_back(newDisplayObject);
	}//End for
}//End BuildDisplayList

void Game::BuildDisplayChunk(ChunkObject* SceneChunk)
{
	//Populate our local display chunk with all the chunk info we need from the object stored in toolmain
	m_displayChunk.PopulateChunkData(SceneChunk);
	m_displayChunk.LoadHeightMap(m_deviceResources);
	m_displayChunk.m_terrainEffect->SetProjection(m_projection);
	m_displayChunk.InitialiseBatch();
}//End BuildDisplayChunk

void Game::SaveDisplayChunk(ChunkObject* SceneChunk)
{
	m_displayChunk.SaveHeightMap();
}//End SaveDisplayChunk

#ifdef DXTK_AUDIO
void Game::NewAudioDevice()
{
    if (m_audEngine && !m_audEngine->IsAudioDevicePresent())
    {
        // Setup a retry in 1 second
        m_audioTimerAcc = 1.f;
        m_retryDefault = true;
    }//End if
}//End NewAudioDevice
#endif
#pragma endregion

#pragma region Direct3D Resources
//These are the resources that depend on the device
void Game::CreateDeviceDependentResources()
{
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto device = m_deviceResources->GetD3DDevice();

    m_states = std::make_unique<CommonStates>(device);

    m_fxFactory = std::make_unique<EffectFactory>(device);
    //Look in the database directory
	m_fxFactory->SetDirectory(L"database/data/");
    //We must set this to false - otherwise, it will share effects based on the initial texture loaded (when the model loads) rather than what we change them to
	m_fxFactory->SetSharing(false);

    m_sprites = std::make_unique<SpriteBatch>(context);

    m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

    m_batchEffect = std::make_unique<BasicEffect>(device);
    m_batchEffect->SetVertexColorEnabled(true);
    
    void const* shaderByteCode;
    size_t byteCodeLength;

    m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

    DX::ThrowIfFailed(
        device->CreateInputLayout(VertexPositionColor::InputElements,
            VertexPositionColor::InputElementCount,
            shaderByteCode, byteCodeLength,
            m_batchInputLayout.ReleaseAndGetAddressOf())
    );

    m_font = std::make_unique<SpriteFont>(device, L"SegoeUI_18.spritefont");

	//m_shape = GeometricPrimitive::CreateTeapot(context, 4.f, 8);

    //SDKMESH has to use clockwise winding with right-handed coordinates, so textures are flipped in the U-axis
    m_model = Model::CreateFromSDKMESH(device, L"tiny.sdkmesh", *m_fxFactory);
	
    //Load textures
    DX::ThrowIfFailed(
        CreateDDSTextureFromFile(device, L"seafloor.dds", nullptr, m_texture1.ReleaseAndGetAddressOf())
    );

    DX::ThrowIfFailed(
        CreateDDSTextureFromFile(device, L"windowslogo.dds", nullptr, m_texture2.ReleaseAndGetAddressOf())
    );
}//End CreateDeviceDependentResources

//Allocate all memory resources that change on a window SizeChanged event
void Game::CreateWindowSizeDependentResources()
{
    auto size = m_deviceResources->GetOutputSize();
    float aspectRatio = float(size.right) / float(size.bottom);
    float fovAngleY = 70.0f * XM_PI / 180.0f;

    //This is a simple example of change that can be made when the app is in portrait or snapped view
    if (aspectRatio < 1.0f)
    {
        fovAngleY *= 2.0f;
    }//End if

    //This sample makes use of a right-handed coordinate system using row-major matrices
    m_projection = Matrix::CreatePerspectiveFieldOfView(
        fovAngleY,
        aspectRatio,
        0.01f,
        1000.0f
    );

    m_batchEffect->SetProjection(m_projection);
}//End CreateWindowSizeDependentResources

void Game::OnDeviceLost()
{
    m_states.reset();
    m_fxFactory.reset();
    m_sprites.reset();
    m_batch.reset();
    m_batchEffect.reset();
    m_font.reset();
    m_shape.reset();
    m_model.reset();
    m_texture1.Reset();
    m_texture2.Reset();
    m_batchInputLayout.Reset();
}//End OnDeviceLost

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}//End OnDeviceRestored
#pragma endregion

std::wstring StringToWCHART(std::string s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}//End StringToWCHART