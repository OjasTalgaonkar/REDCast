#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <windows.h>
#include <winerror.h>
#include <winnt.h>
#include <wrl/client.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#define SHM_NAME "JIT_CORE_OPEN_MEM"
#define SHM_SIZE 65536
#define QUEUE_SIZE 240
#define PACKET_SIZE 
#define SEM_PROD "Global\\SemProd"
#define SEM_CONS "Global\\SemCons"

typedef struct {
  int head;
  int tail;
  int count;
  int lengths[QUEUE_SIZE];               // NEW: to store each packet's length
  char packets[QUEUE_SIZE][PACKET_SIZE]; // Raw packet data
  int shutdown;
} FrameBuffer;

int main() {
  Microsoft::WRL::ComPtr<ID3D11Device> device;
  Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

  HRESULT hr0 =
      D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr,
                        0, D3D11_SDK_VERSION, &device, nullptr, &context);

  if (FAILED(hr0)) {
    // TODO throw some error
  }

  Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
  device.As(&dxgiDevice);

  Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
  dxgiDevice->GetAdapter(&dxgiAdapter);

  Microsoft::WRL::ComPtr<IDXGIOutput> output;
  dxgiAdapter->EnumOutputs(0, &output); // monitor output acquired

  Microsoft::WRL::ComPtr<IDXGIOutput1> output1;
  output.As(&output1);

  Microsoft::WRL::ComPtr<IDXGIOutputDuplication> duplication;
  HRESULT hr = output1->DuplicateOutput(device.Get(), &duplication);

  if (FAILED(hr)) {
    // screen capture not allowed / already in use / wrong GPU
  }

  //--------------------- CAPTURE LOOP -------------------------//

  while (true) {
    DXGI_OUTDUPL_FRAME_INFO frameinfo{};
    Microsoft::WRL::ComPtr<IDXGIResource> resource;

    HRESULT hr = duplication->AcquireNextFrame(16, &frameinfo, &resource);

    if (hr == DXGI_ERROR_WAIT_TIMEOUT) {
        continue; // nothing changed
    }

    if (hr == DXGI_ERROR_ACCESS_LOST) {
        // screen resolution changed, must recreate everything
        break;
    }

    if (FAILED(hr)) {
        break;
    }

    Microsoft::WRL::ComPtr<ID3D11Texture2D> frame;
    resource.As(&frame);

    // ---- USE FRAME HERE ----

    duplication->ReleaseFrame();
  }

  

  
}
