/******************************************************************************
* guacamole - delicious VR                                                   *
*                                                                            *
* Copyright: (c) 2011-2013 Bauhaus-Universität Weimar                        *
* Contact:   felix.lauer@uni-weimar.de / simon.schneegans@uni-weimar.de      *
*                                                                            *
* This program is free software: you can redistribute it and/or modify it    *
* under the terms of the GNU General Public License as published by the Free *
* Software Foundation, either version 3 of the License, or (at your option)  *
* any later version.                                                         *
*                                                                            *
* This program is distributed in the hope that it will be useful, but        *
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY *
* or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   *
* for more details.                                                          *
*                                                                            *
* You should have received a copy of the GNU General Public License along    *
* with this program. If not, see <http://www.gnu.org/licenses/>.             *
*                                                                            *
******************************************************************************/
#define _USE_MATH_DEFINES

#include <gua/guacamole.hpp>
#include <gua/scenegraph/TexturedQuadNode.hpp>
#include <gua/scenegraph/TriMeshNode.hpp>

#include <gua/platform.hpp>

#include <gua/renderer/TriMeshLoader.hpp>
#include <gua/renderer/NURBSLoader.hpp>
#include <gua/renderer/Video3DLoader.hpp>

#include <thread>
#include <chrono>
#include <cmath>


int main(int argc, char** argv) {

  // initialize guacamole
  int argc_d = 0;
  char** argv_d = {};
  gua::init(argc_d, argv_d);

  //gua::ShadingModelDatabase::load_shading_models_from("data/materials/");
  //gua::MaterialDatabase::load_materials_from("data/materials/");
  //gua::TextureDatabase::instance()->load("data/textures/0001MM_diff.jpg");

  // setup scene
  gua::SceneGraph graph("main_scenegraph");

  gua::TriMeshLoader trimeshloader;
  gua::NURBSLoader nurbsloader;
  gua::Video3DLoader videoloader;

  auto video_geode(videoloader.create_geometry_from_file("steppo", argv[1]));
  auto mesh_geode(trimeshloader.create_geometry_from_file("teapot", "data/objects/teapot.obj", "data/materials/Red.gmd", gua::TriMeshLoader::DEFAULTS));
  auto plate_geode(trimeshloader.create_geometry_from_file("plate", "data/objects/plate.obj", "data/materials/White.gmd", gua::TriMeshLoader::DEFAULTS));
  auto nurbs_geode(nurbsloader.create_geometry_from_file("nurbs", "data/objects/teapot.igs", "data/materials/Orange.gmd", gua::NURBSLoader::DEFAULTS));

  auto video = graph.add_node<gua::TransformNode>("/", "video");
  auto mesh = graph.add_node<gua::TransformNode>("/", "mesh");
  auto nurbs = graph.add_node<gua::TransformNode>("/", "nurbs");
  auto plate = graph.add_node<gua::TransformNode>("/", "plate");

  graph.add_node("/video", video_geode);
  graph.add_node("/mesh", mesh_geode);
  graph.add_node("/nurbs", nurbs_geode);
  graph.add_node("/plate", plate_geode);

  auto screen = graph.add_node<gua::ScreenNode>("/", "screen");
  screen->data.set_size(gua::math::vec2(1.6, 0.9));
  screen->translate(0, 0, 6.f);

  auto eye = graph.add_node<gua::TransformNode>("/", "eye");
  eye->translate(-0.05, 0, 7);

  auto eye2 = graph.add_node<gua::TransformNode>("/", "eye2");
  eye2->translate(0.05, 0, 7);

  auto quad = graph.add_node<gua::TexturedQuadNode>("/", "quad");
  quad->translate(0.5f, 0.0, -1.f);
  quad->scale(2.0f);
  quad->set_texture("data/textures/0001MM_diff.jpg");

#if 0

  auto sunlight = graph.add_node<gua::SunLightNode>("/", "sunlight");
  sunlight->data.set_shadow_map_size(1024);
  sunlight->data.set_shadow_offset(0.005f);
  sunlight->data.set_enable_shadows(true);
  sunlight->rotate(-90, 1, 0, 0);
  sunlight->translate(0, 0, -5);
#endif

#if 1
  auto spotlight = graph.add_node<gua::SpotLightNode>("/", "spotlight");
  spotlight->scale(30.0f);
  spotlight->rotate(-70, 1, 0, 0);
  spotlight->translate(2.0, 12.0, 1.0);

  spotlight->data.set_shadow_map_size(800);
  spotlight->data.set_falloff(0.3f);
  spotlight->data.set_shadow_offset(0.005f);
  spotlight->data.set_color({ 1.0f, 1.0f, 1.0f });
  //spotlight->data.set_enable_shadows(true);
  spotlight->data.set_enable_shadows(true);
  spotlight->data.set_enable_specular_shading(true);
  spotlight->data.set_enable_diffuse_shading(true);
#endif

  unsigned width = 1900;
  unsigned height = 1080;

  auto pipe = new gua::Pipeline();

  pipe->config.set_camera(gua::Camera("/eye", "/eye2",
    "/screen", "/screen",
    "main_scenegraph"));
  pipe->config.set_left_resolution(gua::math::vec2ui(width, height));
  pipe->config.set_right_resolution(gua::math::vec2ui(width, height));
  pipe->config.set_enable_fps_display(true);
  //pipe->config.set_enable_stereo(true);
  pipe->config.set_enable_frustum_culling(false);
  pipe->config.set_enable_backface_culling(false);
  pipe->config.set_enable_preview_display(true);
  //pipe->config.set_enable_bbox_display(true);
  //pipe->config.set_enable_ray_display(true);
  pipe->config.set_near_clip(0.2f);
  pipe->config.set_far_clip(1000.0f);
  //pipe->config.set_enable_bbox_display(true);
  pipe->config.set_background_color(gua::utils::Color3f(0.0, 0.0f, 0.0f));

  auto window(new gua::Window);

#if WIN32
  window->config.set_display_name("\\\\.\\DISPLAY1");
#else
  window->config.set_display_name(":0.0");
#endif
  window->config.set_size(gua::math::vec2ui(width, height));
  window->config.set_left_resolution(gua::math::vec2ui(width, height));
  window->config.set_right_resolution(gua::math::vec2ui(width, height));
  //window->config.set_stereo_mode(gua::StereoMode::ANAGLYPH_RED_CYAN);
  window->config.set_enable_vsync(true);

  pipe->set_window(window);

  gua::Renderer renderer({ pipe });

  auto bbox = mesh_geode->get_bounding_box();
  
  mesh->translate(-bbox.center());
  mesh->scale(5.0f / std::sqrt(bbox.size(0)*bbox.size(0) +
    bbox.size(1)*bbox.size(1) +
    bbox.size(2)*bbox.size(2)));
  
  mesh->translate(gua::math::vec3{ -2.0f, -1.5f, -4.0f });

  bbox = nurbs_geode->get_bounding_box();

  nurbs->translate(-bbox.center());
  nurbs->scale(5.0f / std::sqrt(bbox.size(0)*bbox.size(0) +
    bbox.size(1)*bbox.size(1) +
    bbox.size(2)*bbox.size(2)));
  nurbs->rotate(-90, 1, 0, 0);
  nurbs->translate(gua::math::vec3{ 3.0f, -1.5f, -4.0f });

  plate->scale(0.07);
  plate->translate(0.0f, -4.0f, -4.0f);  

  float time_value = 0;

  // application loop
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    scm::math::mat4f ident;
    scm::math::set_identity(ident);
    video->set_transform(ident);

    video->scale(2.0f + std::sin(time_value));
    video->rotate(10.0f*time_value, 0, 1, 0);
    video->translate(0, -2.0, -2.0);

    time_value += 0.01f;

    video_geode->rotate(0.1, 0, 1, 0);
    
    quad->rotate(0.01, 0, 1, 0);

    mesh_geode->rotate(0.3, 0, 1, 0);

    nurbs_geode->rotate(0.3, 0, 0, 1);

    renderer.queue_draw({ &graph });
  }

  return 0;
}

