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
#include <gua/renderer/Video3DLoader.hpp>
#include <gua/renderer/NURBSLoader.hpp>

#include <thread>
#include <chrono>
#include <cmath>

#define TEST_MULTICONTEXT 1
#define TEST_DISTRIBUTION 1

void set_pipe_defaults(gua::Pipeline* pipe, unsigned width, unsigned height)
{
  pipe->config.set_left_resolution(gua::math::vec2ui(width, height));
  pipe->config.set_right_resolution(gua::math::vec2ui(width, height));
  pipe->config.set_enable_fps_display(true);
  pipe->config.set_enable_frustum_culling(true);
  pipe->config.set_enable_backface_culling(true);
  pipe->config.set_enable_preview_display(true);
  pipe->config.set_enable_bbox_display(false);
  pipe->config.set_near_clip(0.2f);
  pipe->config.set_far_clip(1000.0f);
  pipe->config.set_background_color(gua::utils::Color3f(0.0, 0.0f, 0.0f));
}

void set_window_default(gua::Window* window, unsigned width, unsigned height)
{
  window->config.set_size(gua::math::vec2ui(width, height));
  window->config.set_left_resolution(gua::math::vec2ui(width, height));
  window->config.set_right_resolution(gua::math::vec2ui(width, height));
  window->config.set_enable_vsync(true);
}

void connect_node(gua::SceneGraph& graph,
                  std::string const& name,
                  std::shared_ptr<gua::Node> const& node,
                  unsigned ms)
{
  std::chrono::milliseconds dura(ms);
  std::this_thread::sleep_for(dura);

  graph.add_node(name, node);
}



int main(int argc, char** argv) {

  // initialize guacamole
  int argc_d = 0;
  char** argv_d = {};
  gua::init(argc_d, argv_d);

  // setup scene
  gua::SceneGraph graph("main_scenegraph");

  gua::TriMeshLoader trimeshloader;
  gua::NURBSLoader nurbsloader;
  gua::Video3DLoader videoloader;

  auto video_geode(videoloader.create_geometry_from_file("video_geode", "data/kinect_resources/shot_steppo_animation_distributed_daedalos.ks"));
  auto teapot_geode(trimeshloader.create_geometry_from_file("teapot_geode", "data/objects/teapot.obj", "data/materials/Red.gmd", gua::TriMeshLoader::DEFAULTS));
#if 0
  auto plate_geode(trimeshloader.create_geometry_from_file("plate_geode", "data/objects/Medieval/Medieval_City.obj", "data/materials/White.gmd", gua::TriMeshLoader::DEFAULTS | gua::TriMeshLoader::LOAD_MATERIALS));
#else
  auto plate_geode(trimeshloader.create_geometry_from_file("plate_geode", "data/objects/plate.obj", "data/materials/White.gmd", gua::TriMeshLoader::DEFAULTS));
#endif
  //auto plate_geode(trimeshloader.create_geometry_from_file("plate_geode", "data/objects/50Cent.obj", "data/materials/White.gmd", gua::TriMeshLoader::DEFAULTS | gua::TriMeshLoader::LOAD_MATERIALS));
  auto coin_geode(trimeshloader.create_geometry_from_file("plate_geode", "data/objects/50Cent.obj", "data/materials/White.gmd", gua::TriMeshLoader::DEFAULTS | gua::TriMeshLoader::LOAD_MATERIALS));
  auto nurbs_geode(nurbsloader.create_geometry_from_file("nurbs_geode", "data/objects/teapot.igs", "data/materials/Orange.gmd", gua::NURBSLoader::DEFAULTS));

  auto video = graph.add_node<gua::TransformNode>("/", "video");
  auto teapot = graph.add_node<gua::TransformNode>("/", "teapot");
  auto nurbs = graph.add_node<gua::TransformNode>("/", "nurbs");
  auto plate = graph.add_node<gua::TransformNode>("/", "plate");
  auto coin = graph.add_node<gua::TransformNode>("/", "coin");

#if TEST_DISTRIBUTION
#else
  graph.add_node("/video", video_geode);
  graph.add_node("/teapot", teapot_geode);
  graph.add_node("/nurbs", nurbs_geode);
  graph.add_node("/plate", plate_geode);
  graph.add_node("/coin", coin_geode);
#endif

  auto screen = graph.add_node<gua::ScreenNode>("/", "screen");
  screen->data.set_size(gua::math::vec2(1.6, 0.9));
  screen->translate(0, 0, 6.f);

  auto eye = graph.add_node<gua::TransformNode>("/", "eye");
  eye->translate(-0.05, 0, 7);

  auto eye2 = graph.add_node<gua::TransformNode>("/", "eye2");
  eye2->translate(0.05, 0, 7);

  auto eye3 = graph.add_node<gua::TransformNode>("/", "eye3");
  eye3->translate(-0.05, 0, 6.5);

  auto eye4 = graph.add_node<gua::TransformNode>("/", "eye4");
  eye4->translate(0.05, 0, 6.5);

  auto eye5 = graph.add_node<gua::TransformNode>("/", "eye5");
  eye5->translate(-0.05, 0, 7.5);

  auto eye6 = graph.add_node<gua::TransformNode>("/", "eye6");
  eye6->translate(0.05, 0, 7.5);

  auto eye7 = graph.add_node<gua::TransformNode>("/", "eye7");
  eye7->translate(-0.05, 0, 6.7);

  auto eye8 = graph.add_node<gua::TransformNode>("/", "eye8");
  eye8->translate(0.05, 0, 6.7);

  auto quad = graph.add_node<gua::TexturedQuadNode>("/", "quad");
  quad->translate(0.5f, 0.0, -7.f);
  quad->scale(2.0f);
  quad->set_texture("data/textures/0001MM_diff.jpg");


#if 1
  auto spotlight = graph.add_node<gua::SpotLightNode>("/", "spotlight");
  spotlight->scale(30.0f);
  spotlight->rotate(-90, 1, 0, 0);
  spotlight->translate(1.0, 18.0, 1.0);

  spotlight->data.set_shadow_map_size(1024);
  spotlight->data.set_falloff(0.1f);
  spotlight->data.set_shadow_offset(0.005f);
  spotlight->data.set_color({ 1.0f, 1.0f, 1.0f });
  //spotlight->data.set_enable_shadows(true);
  spotlight->data.set_enable_shadows(true);
  spotlight->data.set_enable_specular_shading(true);
  spotlight->data.set_enable_diffuse_shading(true);
#endif

  auto pipe  = new gua::Pipeline();
  auto pipe2 = new gua::Pipeline();
  auto pipe3 = new gua::Pipeline();
  auto pipe4 = new gua::Pipeline();

  pipe->config.set_camera(gua::Camera("/eye", "/eye2",
    "/screen", "/screen",
    "main_scenegraph"));
  pipe2->config.set_camera(gua::Camera("/eye3", "/eye4",
    "/screen", "/screen",
    "main_scenegraph"));
  pipe3->config.set_camera(gua::Camera("/eye5", "/eye6",
    "/screen", "/screen",
    "main_scenegraph"));
  pipe4->config.set_camera(gua::Camera("/eye7", "/eye8",
    "/screen", "/screen",
    "main_scenegraph"));

  unsigned width = 1600;
  unsigned height = 1200;

  set_pipe_defaults(pipe , width, height); 
  set_pipe_defaults(pipe2, width/2, height/2); 
  set_pipe_defaults(pipe3, width/3, height/3);
  set_pipe_defaults(pipe4, width/4, height/4);

  auto window (new gua::Window);
  auto window2(new gua::Window);
  auto window3(new gua::Window);
  auto window4(new gua::Window);

#if WIN32
  window->config.set_display_name("\\\\.\\DISPLAY1");
  window2->config.set_display_name("\\\\.\\DISPLAY1");
  window3->config.set_display_name("\\\\.\\DISPLAY1");
  window4->config.set_display_name("\\\\.\\DISPLAY1");
#else
  window->config.set_display_name(":0.0");
  window2->config.set_display_name(":0.0");
  window3->config.set_display_name(":0.0");
  window4->config.set_display_name(":0.0");
#endif

  set_window_default(window,  width, height);
  set_window_default(window2, width/2, height/2);
  set_window_default(window3, width/3, height/3);
  set_window_default(window4, width/4, height/4);
  
  pipe->set_window(window);
  pipe2->set_window(window2);
  pipe3->set_window(window3);
  pipe4->set_window(window4);

#if TEST_MULTICONTEXT
  gua::Renderer renderer({ pipe, 
                           pipe2, 
                           pipe3, 
                           pipe4
                         });
#else
  gua::Renderer renderer({ pipe });
#endif

  // transform teapot
  auto bbox = teapot_geode->get_bounding_box();
  teapot->translate(-bbox.center());
  teapot->scale(5.0f / std::sqrt(bbox.size(0)*bbox.size(0) +
    bbox.size(1)*bbox.size(1) +
    bbox.size(2)*bbox.size(2)));
  teapot->translate(gua::math::vec3{ -2.0f, -1.5f, -4.0f });

  // tranform nurbs
  bbox = nurbs_geode->get_bounding_box();
  nurbs->translate(-bbox.center());
  nurbs->scale(5.0f / std::sqrt(bbox.size(0)*bbox.size(0) +
    bbox.size(1)*bbox.size(1) +
    bbox.size(2)*bbox.size(2)));
  nurbs->rotate(-90, 1, 0, 0);
  nurbs->translate(gua::math::vec3{ 3.0f, -1.5f, -4.0f });

  // transform plate
  plate->scale(0.07);
  plate->translate(0.0f, -4.0f, -4.0f);  

  coin->translate(0.0f, -1.0f, -4.0f);
  coin_geode->translate(0.0f, -0.2f, -0.0f);

  float time_value = 0;

#if TEST_DISTRIBUTION
  std::thread t1(std::bind(connect_node, std::ref(graph), "/teapot", teapot_geode, 2000));
  std::thread t2(std::bind(connect_node, std::ref(graph), "/plate", plate_geode, 4000));
  std::thread t3(std::bind(connect_node, std::ref(graph), "/coin", coin_geode, 8000));
  std::thread t4(std::bind(connect_node, std::ref(graph), "/nurbs", nurbs_geode, 8000));
  std::thread t5(std::bind(connect_node, std::ref(graph), "/video", video_geode, 10000));
#endif

  // application loop
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

   time_value += 0.01f;

   quad->rotate(0.03, 0, 1, 0);
   
   teapot_geode->rotate(0.3, 0, 1, 0);
   
   nurbs_geode->rotate(0.3, 0, 0, 1);

   plate->translate(-plate->get_bounding_box().center());
   plate->rotate(0.3f, 0, 1, 0);
   plate->translate(plate->get_bounding_box().center());
    
   coin_geode->rotate(0.6f, 0, 1, 0);
   coin_geode->rotate(0.8f, 1, 0, 0);

    renderer.queue_draw({ &graph });
  }

  return 0;
}
