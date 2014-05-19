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

#include <thread>
#include <chrono>

int main(int argc, char** argv) {

  // initialize guacamole
  gua::init(argc, argv);

  //gua::ShadingModelDatabase::load_shading_models_from("data/materials/");
  gua::MaterialDatabase::load_materials_from("data/materials/");

  // setup scene
  gua::SceneGraph graph("main_scenegraph");

  gua::TriMeshLoader loader;
  auto teapot_geometry(loader.create_geometry_from_file("teapot", "data/objects/teapot.obj", "data/materials/Red.gmd", gua::TriMeshLoader::NORMALIZE_POSITION | gua::TriMeshLoader::NORMALIZE_SCALE));
  auto teapot = graph.add_node("/", teapot_geometry);

  auto light = graph.add_node<gua::PointLightNode>("/", "light");
  light->scale(5.f);
  light->translate(0, 1.f, 1.f);

  auto screen = graph.add_node<gua::ScreenNode>("/", "screen");
  screen->data.set_size(gua::math::vec2(1.6f, 0.9f));

  auto eye = graph.add_node<gua::TransformNode>("/screen", "eye");
  eye->translate(0, 0, 1.5);

  auto pipe = new gua::Pipeline();
  pipe->config.set_camera(gua::Camera("/screen/eye", "/screen/eye", "/screen", "/screen", "main_scenegraph"));
  pipe->config.set_enable_fps_display(true);
  pipe->set_window(new gua::Window());

  gua::Renderer renderer({pipe});

  // application loop
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100/6));

	  teapot->rotate(0.1, 0, 1, 0);

    renderer.queue_draw({&graph});
  }

  return 0;
}
