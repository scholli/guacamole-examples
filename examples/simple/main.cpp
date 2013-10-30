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

#include <gua/guacamole.hpp>

#include <gua/platform.hpp>

#include <thread>
#include <chrono>

int main(int argc, char** argv) {

  // initialize guacamole
  gua::init(argc, argv);

  gua::ShadingModelDatabase::load_shading_models_from("data/materials/");
  gua::MaterialDatabase::load_materials_from("data/materials/");

  // setup scene
  gua::SceneGraph graph("main_scenegraph");

  gua::GeometryLoader loader;
  auto teapot_geometry(loader.create_geometry_from_file("teapot", "data/objects/teapot.obj", "Red", gua::GeometryLoader::NORMALIZE_POSITION | gua::GeometryLoader::NORMALIZE_SCALE));

  auto teapot = graph.add_node("/", teapot_geometry);

  auto light = graph.add_node<gua::PointLightNode>("/", "light");
  light->scale(5.f);
  light->translate(0, 1.f, 1.f);

  auto screen = graph.add_node<gua::ScreenNode>("/", "screen");
  screen->data.set_size(gua::math::vec2(1.6f, 0.9f));
  screen->translate(0, 0, 0.f);

  auto view = graph.add_node<gua::ViewNode>("/screen", "view");
  view->data.set_stereo_width(0.1f);
  view->translate(0, 0, 1.5);

  auto pipe = new gua::Pipeline();
  pipe->config.set_camera(gua::Camera("/screen/view", "/screen", "main_scenegraph"));
  pipe->config.set_enable_fps_display(true);
  pipe->set_window(new gua::Window());

  gua::Renderer renderer({pipe});

  // application loop
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    teapot->rotate(0.1, 0, 1, 0);

    renderer.queue_draw({&graph});
  }

  return 0;
}
