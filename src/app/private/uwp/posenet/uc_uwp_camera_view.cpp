#include "pch.h"
#include "uc_uwp_camera_view.h"

#include "../uc_uwp_gx_render_context.h"
#include "../uc_uwp_device_resources.h"

#include <uc_dev/gx/dx12/cmd/profiler.h>
#include <uc_dev/gx/dx12/dx12.h>

#include <autogen/shaders/camera_view_graphics.h>

#include <winrt/Windows.Media.h>
#include <winrt/Windows.Media.Capture.h>
#include <winrt/Windows.Media.Capture.Frames.h>
#include <future>

namespace uc
{
    namespace uwp
    {
        namespace gxu
        {
            namespace
            {
                std::future<void> capture_camera(camera_view* view)
                {
                    MediaCapture capture;

                    auto groups = co_await Frames::MediaFrameSourceGroup::FindAllAsync();

                    for (auto&& group : groups)
                    {
                        auto infos = group.SourceInfos();

                        for (auto&& info : infos)
                        {
                            if (info.SourceKind() == Frames::MediaFrameSourceKind::Color)
                            {
                                MediaCaptureInitializationSettings settings;

                                settings.SourceGroup() = group;
                                settings.SharingMode(MediaCaptureSharingMode::SharedReadOnly);
                                settings.MemoryPreference(MediaCaptureMemoryPreference::Cpu);
                                settings.StreamingCaptureMode(StreamingCaptureMode::Video);

                                co_await capture.InitializeAsync(settings);

                                auto source = capture.FrameSources().First();
                                auto reader = co_await capture.CreateFrameReaderAsync(source.Current().Value());
                                auto status = co_await reader.StartAsync();

                                if (status == Frames::MediaFrameReaderStartStatus::Success )
                                {
                                    view->set_media_capture(capture);
                                    view->set_frame_arrived(reader);
                                    view->set_media_frame_reader(reader);
                                }

                                break;
                            }
                        }
                    }

                    co_return;
                }
            }

            void camera_view::set_media_capture(const MediaCapture& v)
            {
                m_media_capture = v;
            }

            void camera_view::set_media_frame_reader(const Frames::MediaFrameReader& v )
            {
                m_frame_reader = v;
            }

            //void camera_view::set_frame_arrived(Frames::MediaFrameReader::FrameArrived_revoker&& v)
            void camera_view::set_frame_arrived(const Frames::MediaFrameReader& reader)
            {
                Frames::MediaFrameReader r = reader;
                m_revoker = r.FrameArrived(winrt::auto_revoke, { this, &camera_view::on_frame_arrived });
            }

            camera_view::camera_view(initialize_context* resources) : m_frame_reader(nullptr)
            {
                m_pso = gx::dx12::create_pso(resources->m_resources->device_d2d12(), resources->m_resources->resource_create_context(), gx::dx12::camera_view_graphics::create_pso);

                capture_camera(this);
            }

            void camera_view::render(gx::dx12::gpu_graphics_command_context* graphics)
            {
                auto profile_event = gx::dx12::make_profile_event(graphics, L"Camera View");
                D3D12_VERTEX_BUFFER_VIEW v = {};
                D3D12_INDEX_BUFFER_VIEW  i = {};
                graphics->set_vertex_buffer(0, v);
                graphics->set_index_buffer(i);
                graphics->set_pso(m_pso);
                graphics->draw(4);
            }

            void camera_view::update(update_context* ctx)
            {
                ctx;//
            }

            void camera_view::on_frame_arrived(const winrt::Windows::Foundation::IInspectable&, const Frames::MediaFrameArrivedEventArgs& )
            {
                //__debugbreak();
            }
        }
    }
}