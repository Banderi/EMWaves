#include "fields.h"
#include <cstdlib>

void particle::push_history() {
    awake = true; // TODO

    // switch back and forth from chunk A to chunk B
    if (history_count < MAX_STATE_HISTORY_CHUNK)
        history_A[history_count] = state;
    else if (history_count < MAX_STATE_HISTORY)
        history_B[history_count - MAX_STATE_HISTORY_CHUNK] = state;

    // advance counter!
    history_count++;
    if (history_count >= MAX_STATE_HISTORY)
        history_count -= MAX_STATE_HISTORY;
}
void particle::move(double newlifestamp, Vector3 newpos) {
    push_history();
    state.velocity = newpos - state.position;
    state.position = newpos;
    state.lifestamp = newlifestamp;
}

particle_state particle::get_state() {
    return state;
}
particle_state particle::get_state(double life) {
    double closest_matching_lifestamp = 10;
    particle_state *closest_matching_state = &history_A[0];

    for (int i = 0; i < MAX_STATE_HISTORY; ++i) {
        particle_state *ss;
        if (i < MAX_STATE_HISTORY_CHUNK)
            ss = &history_A[i];
        else if (i < MAX_STATE_HISTORY)
            ss = &history_B[i - MAX_STATE_HISTORY_CHUNK];
        else
            return *closest_matching_state; // this should NEVER happen????

        double matching_lifestamp_diff = abs(ss->lifestamp - life);
        if (matching_lifestamp_diff < closest_matching_lifestamp) {
            closest_matching_lifestamp = matching_lifestamp_diff;
            closest_matching_state = ss;
        }
    }
    return *closest_matching_state;
}
particle_state particle::get_signal_impingement(Vector3 point, double signal_propagation_speed) {
    double closest_matching_signal_stamp = 10;
    particle_state *closest_matching_state = &history_A[0];

    double current_time = get_state().lifestamp;

    int METHOD = 0;
    switch (METHOD) {
        case 0: {
            for (int i = 0; i < MAX_STATE_HISTORY; ++i) {
                particle_state *ss;
                if (i < MAX_STATE_HISTORY_CHUNK)
                    ss = &history_A[i];
                else if (i < MAX_STATE_HISTORY)
                    ss = &history_B[i - MAX_STATE_HISTORY_CHUNK];
                else
                    return *closest_matching_state; // this should NEVER happen????

                double time_diff = abs(current_time - ss->lifestamp);
                double ideal_ssdiff = signal_propagation_speed * time_diff;
                double actual_ssdiff = Vector3::Distance(ss->position, point);

                double matching_signal_stamp_diff = abs(actual_ssdiff - ideal_ssdiff);
                if (matching_signal_stamp_diff < closest_matching_signal_stamp) {
                    closest_matching_signal_stamp = matching_signal_stamp_diff;
                    closest_matching_state = ss;
                }
            }
            //    for i in velocity_history.size():
            //    var H = velocity_history[i]
            //    var t = H[0]
            //    var p = H[1]
            //
            //    var time_difference = abs(t - life)
            //    var ideal_distance_at_time = null
            //    var actual_distance = null
            //    ideal_distance_at_time = SPEED_OF_LIGHT * time_difference
            //    ideal_distance_at_time = ideal_distance_at_time * ideal_distance_at_time
            //    actual_distance = (point - p).length_squared()
            //    var distance_accuracy = abs(ideal_distance_at_time - actual_distance)
            //
            //    if distance_accuracy < closest_accuracy:
            //    closest_E = H[2]
            //    closest_accuracy = distance_accuracy
            break;
        }
        case 5: {
            //# fetch cached index, or start from 0.
            //    var i = 0 # OLDEST point in the history. (0)
            //    if cache_id == null || cache_id > smart_index_cache_array.size() - 1: # new/invalid cache id?
            //        cache_id = smart_index_cache_array.size()
            //          smart_index_cache_array.push_back(null) # add a new (default empty) to the cache array
            //    else:
            //    var cc = smart_index_cache_array[cache_id]
            //    i = cc[0]
            //    pass
            //
            //# temp container for the cache.
            //    var index_to_be_cached = i
            //    var rr_next
            //    var rr_curr
            //    var rr_prev
            //
            //    ### STEP 1: start from the **CACHED** or the **OLDEST** index (0) in the history.
            //            rr_curr = method_2_test_i(i, point, -1)
            //
            //# no need to check; the closest accuracy at this point will always be the initial 1000.0
            //    closest_accuracy = rr_curr
            //    index_to_be_cached = i
            //
            //    if abs(closest_accuracy) <= 0.1: # already hit within min accuracy, return.
            //    pass # ...
            //    else:
            //
            //    ### STEP 2: check the **PREVIOUS** index in the history.
            //    if i > 0:
            //    rr_prev = method_2_test_i(i - 1, point, -1) # PREVIOUS point in the history. (-1)
            //
            //# better than the last one!!
            //    if abs(rr_prev) < abs(closest_accuracy):
            //    closest_accuracy = rr_prev
            //    index_to_be_cached = i - 1
            //
            //    ### STEP 3: check the **NEXT** index in the history.
            //    if i < velocity_history.size() - 1:
            //    rr_next = method_2_test_i(i + 1, point, -1) # NEXT point in the history. (+1)
            //
            //# the NEXT is the BEST of the triplet, so... we're done!
            //    if abs(rr_next) < abs(closest_accuracy):
            //    closest_accuracy = rr_next
            //    index_to_be_cached = i + 1
            break;
        }
    }
    return *closest_matching_state;
}

////////

Vector3 electron::get_E_impingement(Vector3 point, double signal_propagation_speed) {
    return get_signal_impingement(point, signal_propagation_speed).velocity;
}

////////

field_system Fields;

Vector3 field_system::get_E_impingement(Vector3 point, double signal_propagation_speed) {
    Vector3 E;
    for (auto & g_electron : Fields.g_electrons)
        if (g_electron.is_awake())
            E += g_electron.get_E_impingement(point, signal_propagation_speed);
//    for (int e = 0; e < MAX_PARTICLES_IN_FIELD; ++e)
//        if (Fields.g_electrons[e].is_awake())
//            E += Fields.g_electrons[e].get_E_impingement(point, signal_propagation_speed);
    return E;
}