#include "init.h"

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
    godot::Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
    godot::Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
    godot::Godot::nativescript_init(handle);

    godot::register_class<godot::GDExample>();
}

//////////

using namespace godot;

void GDExample::_register_methods() {
    register_method("_process", &GDExample::_process);
    register_property<GDExample, float>("amplitude", &GDExample::amplitude, 10.0);
    register_property<GDExample, float>("speed", &GDExample::set_speed, &GDExample::get_speed, 1.0);

    register_signal<GDExample>((char *)"position_changed", "node", GODOT_VARIANT_TYPE_OBJECT, "new_pos", GODOT_VARIANT_TYPE_VECTOR2);
}

GDExample::GDExample() {
}

GDExample::~GDExample() {
    // add your cleanup here
}

void GDExample::_init() {
    // initialize any variables here
    time_passed = 0.0;
    amplitude = 10.0;
    speed = 1.0;
}

void GDExample::_process(float delta) {
    time_passed += speed * delta;

    Vector2 new_position = Vector2(
            amplitude + (amplitude * sin(time_passed * 2.0)),
            amplitude + (amplitude * cos(time_passed * 1.5))
    );

    set_position(new_position);

    time_emit += delta;
    if (time_emit > 1.0) {
        emit_signal("position_changed", this, new_position);

        time_emit = 0.0;
    }
}

void GDExample::set_speed(float p_speed) {
    speed = p_speed;
}

float GDExample::get_speed() {
    return speed;
}








///*************************************************************************/
///*  init.cpp                                                             */
///*************************************************************************/
///*                       This file is part of:                           */
///*                           GODOT ENGINE                                */
///*                      https://godotengine.org                          */
///*************************************************************************/
///* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
///* Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).   */
///*                                                                       */
///* Permission is hereby granted, free of charge, to any person obtaining */
///* a copy of this software and associated documentation files (the       */
///* "Software"), to deal in the Software without restriction, including   */
///* without limitation the rights to use, copy, modify, merge, publish,   */
///* distribute, sublicense, and/or sell copies of the Software, and to    */
///* permit persons to whom the Software is furnished to do so, subject to */
///* the following conditions:                                             */
///*                                                                       */
///* The above copyright notice and this permission notice shall be        */
///* included in all copies or substantial portions of the Software.       */
///*                                                                       */
///* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
///* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
///* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
///* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
///* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
///* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
///* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
///*************************************************************************/
//
//#ifdef __cplusplus
//#define CEXTERN extern "C"
//#else
//#define CEXTERN
//#endif
//
//#include <Godot.hpp>
//#include <Reference.hpp>
//
//using namespace godot;
//
//class SimpleClass : public Reference {
//GODOT_CLASS(SimpleClass, Reference);
//
//public:
//    SimpleClass() {}
//
//    /** `_init` must exist as it is called by Godot. */
////    void _init() {
////        _name = String("SimpleClass");
////        _value = 0;
////    }
//
////    void test_void_method() {
////        Godot::print("This is test");
////    }
//
////    Variant method(Variant arg) {
////        Variant ret;
////        ret = arg;
////
////        return ret;
////    }
//
//    static void _register_methods() {
////        register_method("method", &SimpleClass::method);
////
////        /**
////         * The line below is equivalent to the following GDScript export:
////         *	 export var _name = "SimpleClass"
////         **/
////        register_property<SimpleClass, String>("name", &SimpleClass::_name, String("SimpleClass"));
////
////        /** Alternatively, with getter and setter methods: */
////        register_property<SimpleClass, int>("value", &SimpleClass::set_value, &SimpleClass::get_value, 0);
////
////        /** Registering a signal: **/
////        register_signal<SimpleClass>("signal_name0"); // windows: error C2668: 'godot::register_signal': ambiguous call to overloaded function
////        register_signal<SimpleClass>("signal_name1", "string_argument", GODOT_VARIANT_TYPE_STRING);
//    }
//
////    String _name;
////    int _value;
//
////    void set_value(int p_value) {
////        _value = p_value;
////    }
////
////    int get_value() const {
////        return _value;
////    }
//};
//
///** GDNative Initialize **/
//CEXTERN void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
//    godot::Godot::gdnative_init(o);
//    Godot::gdnative_init(o);
//}
//
/////** GDNative Terminate **/
////CEXTERN void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
//////    godot::Godot::gdnative_terminate(o);
////}
////
/////** NativeScript Initialize **/
//CEXTERN void GDN_EXPORT godot_nativescript_init(void *handle) {
////    godot::Godot::nativescript_init(handle);
////
////    godot::register_class<SimpleClass>();
//}