#pragma once
#include <dxgi.h>

const char *dx_error_string(HRESULT code) {
	switch (code) {
		case DXGI_ERROR_ACCESS_DENIED:
			return "You tried to use a resource to which you did not have the required access privileges. This error is most typically caused when you write to a shared resource with read-only access.";
		case DXGI_ERROR_ACCESS_LOST:
			return "The desktop duplication interface is invalid. The desktop duplication interface typically becomes invalid when a different type of image is displayed on the desktop.";
		case DXGI_ERROR_ALREADY_EXISTS:
			return "The desired element already exists. This is returned by DXGIDeclareAdapterRemovalSupport if it is not the first time that the function is called.";
		case DXGI_ERROR_CANNOT_PROTECT_CONTENT:
			return "DXGI can't provide content protection on the swap chain. This error is typically caused by an older driver, or when you use a swap chain that is incompatible with content protection.";
		case DXGI_ERROR_DEVICE_HUNG:
			return "The application's device failed due to badly formed commands sent by the application. This is an design-time issue that should be investigated and fixed.";
		case DXGI_ERROR_DEVICE_REMOVED:
			return "The video card has been physically removed from the system, or a driver upgrade for the video card has occurred. The application should destroy and recreate the device. For help debugging the problem, call ID3D10Device::GetDeviceRemovedReason.";
		case DXGI_ERROR_DEVICE_RESET:
			return "The device failed due to a badly formed command. This is a run-time issue; The application should destroy and recreate the device.";
		case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
			return "The driver encountered a problem and was put into the device removed state.";
		case DXGI_ERROR_FRAME_STATISTICS_DISJOINT:
			return "An event (for example, a power cycle) interrupted the gathering of presentation statistics.";
		case DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE:
			return "The application attempted to acquire exclusive ownership of an output, but failed because some other application (or device within the application) already acquired ownership.";
		case DXGI_ERROR_INVALID_CALL:
			return "The application provided invalid parameter data, this must be debugged and fixed before the application is released.DXGI_ERROR_MORE_DATA 0x887A0003 The buffer supplied by the application is not big enough to hold the requested data.DXGI_ERROR_NAME_ALREADY_EXISTS 0x887A002C The supplied name of a resource in a call to IDXGIResource1::CreateSharedHandle is already associated with some other resource.DXGI_ERROR_NONEXCLUSIVE 0x887A0021 A global counter resource is in use, and the Direct3D device can't currently use the counter resource. DXGI_ERROR_NOT_CURRENTLY_AVAILABLE 0x887A0022 The resource or request is not currently available, but it might become available later.DXGI_ERROR_NOT_FOUND 0x887A0002 When calling IDXGIObject::GetPrivateData, the GUID passed in is not recognized as one previously passed to IDXGIObject::SetPrivateData or IDXGIObject::SetPrivateDataInterface.When calling IDXGIFactory::EnumAdapters or IDXGIAdapter::EnumOutputs, the enumerated ordinal is out of range.DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED 0x887A0023 Reserved DXGI_ERROR_REMOTE_OUTOFMEMORY 0x887A0024 Reserved DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE 0x887A0029 The DXGI output(monitor) to which the swap chain content was restricted is now disconnected or changed.DXGI_ERROR_SDK_COMPONENT_MISSING 0x887A002D The operation depends on an SDK component that is missing or mismatched.DXGI_ERROR_SESSION_DISCONNECTED 0x887A0028 The Remote Desktop Services session is currently disconnected.DXGI_ERROR_UNSUPPORTED 0x887A0004 The requested functionality is not supported by the device or the driver.DXGI_ERROR_WAIT_TIMEOUT 0x887A0027 The time - out interval elapsed before the next desktop frame was available.DXGI_ERROR_WAS_STILL_DRAWING 0x887A000A The GPU was busy at the moment when a call was made to perform an operation, and did not execute or schedule the operation.";
		case D3D11_ERROR_FILE_NOT_FOUND:
			return "The file was not found.";
		case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
			return "There are too many unique instances of a particular type of state object.";
		case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
			return "There are too many unique instances of a particular type of view object.";
		case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
			return "The first call to ID3D11DeviceContext::Map after either ID3D11Device::CreateDeferredContext or ID3D11DeviceContext::FinishCommandList per Resource was not D3D11_MAP_WRITE_DISCARD.";
		case E_INVALIDARG:
			return "Invalid Argument";
		case E_OUTOFMEMORY:
			return "Out of Memory";
		case S_OK:
			return "The method succeeded without an error.";
	}
	return "-unknown-";
}
