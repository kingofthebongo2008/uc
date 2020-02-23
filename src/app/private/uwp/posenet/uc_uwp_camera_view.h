#pragma once
#include <uc_dev/util/noncopyable.h>

#include <autogen/shaders/camera_view_graphics.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>

#include <winrt/Windows.Media.h>
#include <winrt/Windows.Media.Capture.h>
#include <winrt/Windows.Media.Capture.Frames.h>
#include <shared_mutex>


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

                struct frame_data
                {
                    frame_data() : m_frame(nullptr) {}
					Frames::MediaFrameReference m_frame;
					uint32_t                    m_index;
                };

                camera_view(initialize_context* c);
                void initialize();
                void render(gx::dx12::gpu_graphics_command_context* ctx);
                void update(update_context* ctx);

                void set_media_parameters(const MediaCapture& capture, const Frames::MediaFrameReader& reader);

                private:
                gx::dx12::camera_view_graphics::graphics_pipeline_state* m_pso;
                MediaCapture                                             m_media_capture;
                Frames::MediaFrameReader                                 m_frame_reader;
                Frames::MediaFrameReader::FrameArrived_revoker           m_revoker;
                std::shared_mutex                                        m_lock;
                uint32_t m_frame_index                                   = 0;
                frame_data                                               m_frame_data;

                void on_frame_arrived(const winrt::Windows::Foundation::IInspectable&, const Frames::MediaFrameArrivedEventArgs& args);
            };
        }
    }
    
}