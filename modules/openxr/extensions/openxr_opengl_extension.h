/*************************************************************************/
/*  openxr_opengl_extension.h                                            */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef OPENXR_OPENGL_EXTENSION_H
#define OPENXR_OPENGL_EXTENSION_H

#ifdef GLES3_ENABLED

#include "core/templates/vector.h"
#include "openxr_extension_wrapper.h"

#include "../openxr_api.h"
#include "../util.h"

#ifdef ANDROID_ENABLED
#define XR_USE_GRAPHICS_API_OPENGL_ES
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#else
#define XR_USE_GRAPHICS_API_OPENGL
#endif

#ifdef WINDOWS_ENABLED
// Including windows.h here is absolutely evil, we shouldn't be doing this outside of platform
// however due to the way the openxr headers are put together, we have no choice.
#include <windows.h>
#endif

#ifdef X11_ENABLED
#include OPENGL_INCLUDE_H
#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <X11/Xlib.h>
#endif

#ifdef ANDROID_ENABLED
// The jobject type from jni.h is used by openxr_platform.h on Android.
#include <jni.h>
#endif

// include platform dependent structs
#include <openxr/openxr_platform.h>

class OpenXROpenGLExtension : public OpenXRGraphicsExtensionWrapper {
public:
	OpenXROpenGLExtension(OpenXRAPI *p_openxr_api);
	virtual ~OpenXROpenGLExtension() override;

	virtual void on_instance_created(const XrInstance p_instance) override;
	virtual void *set_session_create_and_get_next_pointer(void *p_next_pointer) override;

	virtual void get_usable_swapchain_formats(Vector<int64_t> &p_usable_swap_chains) override;
	virtual void get_usable_depth_formats(Vector<int64_t> &p_usable_swap_chains) override;
	virtual String get_swapchain_format_name(int64_t p_swapchain_format) const override;
	virtual bool get_swapchain_image_data(XrSwapchain p_swapchain, int64_t p_swapchain_format, uint32_t p_width, uint32_t p_height, uint32_t p_sample_count, uint32_t p_array_size, void **r_swapchain_graphics_data) override;
	virtual void cleanup_swapchain_graphics_data(void **p_swapchain_graphics_data) override;
	virtual bool create_projection_fov(const XrFovf p_fov, double p_z_near, double p_z_far, Projection &r_camera_matrix) override;
	virtual RID get_texture(void *p_swapchain_graphics_data, int p_image_index) override;

private:
	static OpenXROpenGLExtension *singleton;

#ifdef WIN32
	static XrGraphicsBindingOpenGLWin32KHR graphics_binding_gl;
#elif ANDROID_ENABLED
	static XrGraphicsBindingOpenGLESAndroidKHR graphics_binding_gl;
#else
	static XrGraphicsBindingOpenGLXlibKHR graphics_binding_gl;
#endif

	struct SwapchainGraphicsData {
		bool is_multiview;
		Vector<RID> texture_rids;
	};

	bool check_graphics_api_support(XrVersion p_desired_version);

#ifdef ANDROID_ENABLED
	EXT_PROTO_XRRESULT_FUNC3(xrGetOpenGLESGraphicsRequirementsKHR, (XrInstance), p_instance, (XrSystemId), p_system_id, (XrGraphicsRequirementsOpenGLESKHR *), p_graphics_requirements)
#else
	EXT_PROTO_XRRESULT_FUNC3(xrGetOpenGLGraphicsRequirementsKHR, (XrInstance), p_instance, (XrSystemId), p_system_id, (XrGraphicsRequirementsOpenGLKHR *), p_graphics_requirements)
#endif
	EXT_PROTO_XRRESULT_FUNC4(xrEnumerateSwapchainImages, (XrSwapchain), p_swapchain, (uint32_t), p_image_capacity_input, (uint32_t *), p_image_count_output, (XrSwapchainImageBaseHeader *), p_images)
};

#endif // GLES3_ENABLED

#endif // OPENXR_OPENGL_EXTENSION_H
