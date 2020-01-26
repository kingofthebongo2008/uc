#pragma once

#include <memory>
#include <vector>

#include <uc_dev/mem/align.h>
#include <uc_dev/gx/dx12/dx12.h>
#include <uc_dev/gx/blue_noise/moment_shadow_maps_blue_noise.h>
#include <uc_dev/gx/geo/indexed_geometry.h>
#include <uc_dev/gx/anm/animation_instance.h>
#include <uc_dev/gx/structs.h>

#include "uc_uwp_gx_render_world_moment_shadows_data.h"

#include "posenet/uc_uwp_camera_view.h"

namespace uc
{
    namespace uwp
    {
        namespace gxu
        {
            class render_world_non_linear_moment_shadow_maps_64 : public render_world_moment_shadows_data
            {

                using base = render_world_moment_shadows_data;

            public:

                render_world_non_linear_moment_shadow_maps_64(initialize_context* ctx);
                virtual ~render_world_non_linear_moment_shadow_maps_64();

            private:

                std::unique_ptr< submitable >do_render(render_context* ctx) override;
                std::unique_ptr< submitable >do_render_shadows(shadow_render_context* ctx) override;

                std::unique_ptr<gx::blue_noise::ldr_rg01_64x64> m_blue_noise;

                std::unique_ptr<camera_view>                    m_camera_view;
            };
        }
    }
}

