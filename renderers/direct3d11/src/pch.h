
#ifndef __GX_D3D11_PCH_H__
#define __GX_D3D11_PCH_H__

#include <iostream>

#include <string>
#include <vector>
#include <unordered_map>

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

namespace Geoxide {
	using namespace DirectX;
	using namespace Microsoft::WRL;
}

#include <Geoxide/Platform.h>
#include <Geoxide/Log.h>
#include <Geoxide/Math3D.h>

#include "Geoxide/D3D11RendererBase.h"

#endif // !__GEOXIDE_PCH_H__
