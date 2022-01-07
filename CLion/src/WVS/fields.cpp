#include "fields.h"

#define MAX_PROBE_CACHE 400

#include "hash/hashmap.h"
bool probe_cache_allocated = false;
struct hashmap_s hashmap;

void init() {
    const unsigned initial_size = 2;
    if (0 != hashmap_create(initial_size, &hashmap)) {
        // error!
    }
    probe_cache_allocated = true;
}

int check_for_cache(Vector3 point) {
    // the cache map MUST be initialized!!
    if (!probe_cache_allocated)
        init();

    // converts vector into KEY for hashmap browsing
    const char* key = reinterpret_cast<const char *const>(point.data);
    int key_size = 3 * sizeof(double);

    // get stored value (int) associated with vector (KEY)
    void* const element = hashmap_get(&hashmap, key, key_size);
    if (NULL == element) {
        // error!
        return 0;
    }
    return *(int*)element;
}
void record_probe_in_cache(Vector3 point, int index) {
    // the cache map MUST be initialized!!
    if (!probe_cache_allocated)
        return;

    // converts vector into KEY for hashmap indexing
    const char* key = reinterpret_cast<const char *const>(point.data);
    int key_size = 3 * sizeof(double);

    // converts int into *new* heap-stored int container
    auto index_heap = malloc(sizeof(int));
    memcpy((void*)index_heap, (void*)&index, sizeof(int));

    // store int value in the hashmap
    if (0 != hashmap_put(&hashmap, key, key_size, index_heap)) {
        // error!
    }
}

////

void particle::move(double newlifestamp, Vector3 newpos) {
    if (state.lifestamp != -1) {
        double period = newlifestamp - state.lifestamp;
        state.velocity = (newpos - state.position) / period; // only update velocity for states **after** the initial one
    }
    state.position = newpos;
    state.lifestamp = newlifestamp;
    push_history();
}

void particle::push_history() {
    awake = true; // TODO

    // switch back and forth from chunk A to chunk B
    if (history_last_index < MAX_STATE_HISTORY_CHUNK)
        history_A[history_last_index] = state;
    else if (history_last_index < MAX_STATE_HISTORY)
        history_B[history_last_index - MAX_STATE_HISTORY_CHUNK] = state;

    // advance counter!
    history_last_index++;
    if (history_last_index >= MAX_STATE_HISTORY)
        history_last_index -= MAX_STATE_HISTORY;
}
particle_state particle::get_history_absolute(int i) {
    particle_state ss;

    // clamp absolute index within array range
    while (i < 0)
        i += MAX_STATE_HISTORY;
    while (i > MAX_STATE_HISTORY)
        i -= MAX_STATE_HISTORY;

    // convert absolute index to relative (chunk) index and return appropriate field
    if (i < MAX_STATE_HISTORY_CHUNK)
        return history_A[i];
    else
        return history_B[i - MAX_STATE_HISTORY_CHUNK];
}
particle_state particle::get_history_from_front(int i) {
    // clamp absolute index within array range
    if (i < 0)
        i = 0;
    if (i >= MAX_STATE_HISTORY)
        i = MAX_STATE_HISTORY - 1;

    i = history_last_index - 1 - i;
    return get_history_absolute(i);
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

bool particle::check_closest_match(int i, bool absolute, Vector3 point, double signal_propagation_speed, double current_time, particle_state **closest_matching_state, double *closest_matching_signal_stamp, int *closest_matching_id) {
    particle_state ss;
    if (absolute)
        ss = get_history_absolute(i);
    else
        ss = get_history_from_front(i);
    if (ss.lifestamp == -1) // invalid state!
        return false;

    double time_diff = abs(current_time - ss.lifestamp);
    double ideal_ssdiff = signal_propagation_speed * time_diff;
    double actual_ssdiff = Vector3::Distance(ss.position, point);

    double matching_signal_stamp_diff = abs(actual_ssdiff - ideal_ssdiff);
    if (matching_signal_stamp_diff < *closest_matching_signal_stamp) {
        *closest_matching_signal_stamp = matching_signal_stamp_diff;
        **closest_matching_state = ss;
        *closest_matching_id = i;
        return true;
    }
    return false;
}
particle_state particle::get_signal_impingement(Vector3 point, double signal_propagation_speed) {
    double closest_matching_signal_stamp = 100;
    particle_state dummy_state;
    particle_state *closest_matching_state = &dummy_state; //&history_A[0];
    int closest_matching_id;

    double current_time = get_state().lifestamp;

    int METHOD = 0;
    switch (METHOD) {
        case 0: {
            for (int i = 0; i < MAX_STATE_HISTORY; ++i) {
                check_closest_match(i, true, point, signal_propagation_speed, current_time, &closest_matching_state, &closest_matching_signal_stamp, &closest_matching_id);
            }
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
        case 6: {
            int MAX_CHECKS = 2500;
//            int TOTAL_CHECKS = 0;

            int cached_index = check_for_cache(point);
            if (cached_index != 0) {
                int a = 35;
            }
//            cached_index = 0; // TEMP!!!!!!!!
            int START_IDX = fmax(cached_index, cached_index - 2);
            int END_IDX = fmin(START_IDX + MAX_CHECKS, MAX_STATE_HISTORY);

            closest_matching_state = &history_A[START_IDX];
            int closest_matching_id = START_IDX;
            for (int i = START_IDX; i < END_IDX; ++i) {
                particle_state *ss;
                if (i < 0)
                    ss = &history_B[i + MAX_STATE_HISTORY_CHUNK];
                else if (i < MAX_STATE_HISTORY_CHUNK)
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
                    closest_matching_id = i;
                }
            }
            record_probe_in_cache(point, closest_matching_id);
            break;
        }
        case 7: {
//            for (int i = 0; i < MAX_STATE_HISTORY; ++i) {
//                check_closest_match(i, false, point, signal_propagation_speed, current_time, &closest_matching_state, &closest_matching_signal_stamp, &closest_matching_id);
//            }

            bool r = true;
            int i = 0;
            do {
                r = check_closest_match(0 + i, false, point, signal_propagation_speed, current_time, &closest_matching_state, &closest_matching_signal_stamp, &closest_matching_id);
                i++;
            } while (r);

            break;
        }
        case 8: {
            const int MAX_CHECKS = 20;
            const int MAX_CHECKS_HALF = (MAX_CHECKS / 2);
//            const int cached_index_offset = check_for_cache(point);
//            const int START_IDX = fmax(history_last_index - 1 + cached_index_offset, 0);
            const int CACHED_IDX = check_for_cache(point);
            const int START_IDX = fmax(CACHED_IDX - MAX_CHECKS_HALF, 0);
            const int END_IDX = fmin(CACHED_IDX + MAX_CHECKS_HALF, MAX_STATE_HISTORY - 1);

//            bool r = true;
//            int i = 0;
//            do {
//                r = check_closest_match(0 + i, false, point, signal_propagation_speed, current_time, &closest_matching_state, &closest_matching_signal_stamp, &closest_matching_id);
//                i++;
//            } while (r);
            for (int i = START_IDX; i < END_IDX; ++i) {
                check_closest_match(i, false, point, signal_propagation_speed, current_time, &closest_matching_state, &closest_matching_signal_stamp, &closest_matching_id);
            }

            const int CLOSEST_OFFSET = closest_matching_id;
//            if (CLOSEST_OFFSET != 0) {
//                int a = 35;
//            }
//            if (CLOSEST_OFFSET != CACHED_IDX) {
//                int a = 35;
//            }
            record_probe_in_cache(point, CLOSEST_OFFSET);
            break;
        }
    }
    return *closest_matching_state;
}

////////

Vector3 electron::get_E_impingement(Vector3 point, double signal_propagation_speed) {
    auto ss = get_signal_impingement(point, signal_propagation_speed);
    Vector3 E = ss.velocity;
    return E;
}
Vector3 electron::get_B_impingement(Vector3 point, double signal_propagation_speed) {
    auto ss = get_signal_impingement(point, signal_propagation_speed);
    Vector3 E = ss.velocity;
    Vector3 v = Vector3::Normalized(point - ss.position) * signal_propagation_speed;
    return Vector3::Cross(E, v);
}
double electron::get_u_density(Vector3 point, double signal_propagation_speed) {
    auto ss = get_signal_impingement(point, signal_propagation_speed);
    Vector3 E = ss.velocity;
    double Es = Vector3::Magnitude(E);
    return Es * Es;
}

////////

field_system Fields;

Vector3 field_system::get_field_impingement(Vector3 point, double signal_propagation_speed, unsigned int field_component) {
    Vector3 F;

    switch (field_component) {
        case FIELD_ELECTRIC:
            for (auto & g_electron : Fields.g_electrons)
                if (g_electron.is_awake())
                    F += g_electron.get_E_impingement(point, signal_propagation_speed);
            // TODO: photons
            break;
        case FIELD_MAGNETIC:
            for (auto & g_electron : Fields.g_electrons)
                if (g_electron.is_awake())
                    F += g_electron.get_B_impingement(point, signal_propagation_speed);
            // TODO: photons
            break;
        case FIELD_ENERGY_DENSITY:
            for (auto & g_electron : Fields.g_electrons)
                if (g_electron.is_awake())
                    F += Vector3(0, g_electron.get_u_density(point, signal_propagation_speed), 0);
            // TODO: photons
            break;
    }

    return F;
}