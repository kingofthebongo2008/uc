#pragma once
#include <uc_dev/util/noncopyable.h>

#include <autogen/shaders/camera_view_graphics.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>

#include <winrt/Windows.Media.h>
#include <winrt/Windows.Media.Capture.h>
#include <winrt/Windows.Media.Capture.Frames.h>


namespace uc
{
    namespace gx
    {
        namespace dx12
        {
            struct gpu_graphics_command_context;
        }
    }
    
    namespace uwp
    {
        namespace gxu
        {
            struct update_context;
            struct initialize_context;

            using namespace winrt::Windows::Foundation;
            using namespace winrt::Windows::Media::Capture;

            class camera_view : public util::noncopyable
            {

                public:

                camera_view(initialize_context* c);
                void render(gx::dx12::gpu_graphics_command_context* ctx);
                void update(update_context* ctx);

                private:

                gx::dx12::camera_view_graphics::graphics_pipeline_state* m_pso;

                MediaCapture                                             m_media_capture;
                Frames::MediaFrameReader                                 m_frame_reader;
                Frames::MediaFrameReader::FrameArrived_revoker           m_revoker;
                
                public:

                void on_frame_arrived(const winrt::Windows::Foundation::IInspectable&, const Frames::MediaFrameArrivedEventArgs& args);
                //void set_frame_arrived(Frames::MediaFrameReader::FrameArrived_revoker&& v);

                void set_frame_arrived(const Frames::MediaFrameReader& v);
                void set_media_frame_reader(const Frames::MediaFrameReader& v);
                void set_media_capture(const MediaCapture& v);
            };
        }
    }
    
}