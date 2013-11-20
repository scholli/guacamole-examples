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
#include <gua/databases/GeometryDatabase.hpp>
#include <gua/renderer/Volume.hpp>

#include <gua/platform.hpp>

#include <thread>
#include <chrono>
#include <memory>

#include <scm/gl_util/data/analysis/transfer_function/piecewise_function_1d.h>

int main(int argc, char** argv) {

	// initialize guacamole
	gua::init(argc, argv);

	gua::ShadingModelDatabase::load_shading_models_from("data/materials/");
	gua::MaterialDatabase::load_materials_from("data/materials/");

	// setup scene
	gua::SceneGraph graph("main_scenegraph");

	scm::data::piecewise_function_1d<float, float>                 alpha_transfer_head;
	scm::data::piecewise_function_1d<float, scm::math::vec3f>      color_transfer_head;

	scm::data::piecewise_function_1d<float, float>                 alpha_transfer_seis;
	scm::data::piecewise_function_1d<float, scm::math::vec3f>      color_transfer_seis;


	alpha_transfer_head.clear();
	color_transfer_head.clear();

	alpha_transfer_seis.clear();
	color_transfer_seis.clear();

	alpha_transfer_seis.add_stop(0, 1.0f);
	alpha_transfer_seis.add_stop(0.45f, 0.0f);
	alpha_transfer_seis.add_stop(0.50f, 0.0f);
	alpha_transfer_seis.add_stop(0.55f, 0.0f);
	alpha_transfer_seis.add_stop(1.0f, 1.0f);

	alpha_transfer_head.add_stop(0.0f, 0.0f);
	alpha_transfer_head.add_stop(0.3f, 0.0f);
	alpha_transfer_head.add_stop(0.4f, 0.2f);
	alpha_transfer_head.add_stop(0.7f, 0.0f);
	alpha_transfer_head.add_stop(1.0f, 1.0f);

	// blue-grey-orange
	color_transfer_seis.add_stop(0, scm::math::vec3f(1.0f, 0.0f, 0.0f));
	color_transfer_seis.add_stop(0.5, scm::math::vec3f(1.0f, 1.0f, 1.0f));
	color_transfer_seis.add_stop(1.0f, scm::math::vec3f(0.0f, 0.0f, 1.0f));

	// blue-white-red
	color_transfer_head.add_stop(0.0f, scm::math::vec3f(0.0f, 0.0f, 0.0f));
	color_transfer_head.add_stop(0.5f, scm::math::vec3f(0.4f, 0.0f, 0.4f));
	color_transfer_head.add_stop(0.7f, scm::math::vec3f(1.0f, 1.0f, 1.0f));
	color_transfer_head.add_stop(1.0f, scm::math::vec3f(1.0f, 1.0f, 1.0f));



	gua::GeometryLoader loader;

#if 1
	auto teapot_geometry(loader.create_geometry_from_file("teapot", "./data/objects/teapot.obj", "Red", gua::GeometryLoader::NORMALIZE_POSITION | gua::GeometryLoader::NORMALIZE_SCALE));
#else
	std::string file_name_seis = "./data/volumes/mig_dep_0_2200bri.vol";
	auto teapot_geometry(loader.create_geometry_from_file("teapot", file_name_seis, "Volume", gua::GeometryLoader::NORMALIZE_POSITION | gua::GeometryLoader::NORMALIZE_SCALE));
	auto h = std::static_pointer_cast<gua::Volume>(gua::GeometryDatabase::instance()->lookup(file_name_seis));
	h->set_transfer_function(alpha_transfer_head, color_transfer_seis);

#endif

	auto teapot = graph.add_node("/", teapot_geometry);
	teapot->scale(0.5f);
	teapot->translate(-0.4f, 0.f, 0.f);

	//auto volume_g(vloader.create_geometry_from_file("volume", "./data/volumes/mig_dep_0_2200bri.vol", "Volume", gua::GeometryLoader::NORMALIZE_POSITION));

	std::string file_name_vol = "./data/volumes/head_w256_h256_d225_c1_b8.raw";
	auto volume_geometry(loader.create_geometry_from_file("volume", file_name_vol, "Volume", gua::GeometryLoader::NORMALIZE_POSITION));
	auto v = std::static_pointer_cast<gua::Volume>(gua::GeometryDatabase::instance()->lookup(file_name_vol));
	v->set_transfer_function(alpha_transfer_head, color_transfer_head);

	auto volume = graph.add_node("/", volume_geometry);
	volume->scale(0.5f);
	volume->translate(0.4f, 0.f, 0.f);

	auto light = graph.add_node<gua::PointLightNode>("/", "light");
	light->scale(5.f);
	light->translate(0, 1.f, 1.f);

	auto screen = graph.add_node<gua::ScreenNode>("/", "screen");
	screen->data.set_size(gua::math::vec2(1.6f, 0.9f));
	screen->translate(0, 0, 0.f);

	auto eye = graph.add_node<gua::TransformNode>("/screen", "eye");
	eye->translate(0, 0, 1.5);

	auto pipe = new gua::Pipeline();
	pipe->config.set_camera(gua::Camera("/screen/eye", "/screen/eye", "/screen", "/screen", "main_scenegraph"));
	pipe->config.set_enable_fps_display(true);

	auto resolution = gua::math::vec2ui(1920, 1080);

	auto windowConf = gua::Window::Configuration();
	windowConf.set_left_resolution(resolution);
	windowConf.set_right_resolution(resolution);
	windowConf.set_size(resolution);

	pipe->set_window(new gua::Window(windowConf));
	pipe->config.background_color = gua::PipelineConfiguration::background_color_struct(gua::utils::Color3f(0.0f, 0.0f, 0.0f));
	pipe->config.set_left_resolution(resolution);
	pipe->config.set_right_resolution(resolution);

	pipe->print_shaders("./tmp");

	gua::Renderer renderer({ pipe });

	// application loop
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));

		teapot->translate(0.4f, 0.f, 0.f);
		teapot->rotate(0.1, 1, 1, 1);
		teapot->translate(-0.4f, 0.f, 0.f);

		volume->translate(-0.4f, 0.f, 0.f);
		volume->rotate(0.1, 1, 1, 1);
		volume->translate(0.4f, 0.f, 0.f);

		renderer.queue_draw({ &graph });
	}

	return 0;
}
