#include <getopt.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <ubelt/utils.h>
#include <dimappio/core.h>
#include <3030/core.h>

void print_usage() {
    printf("Usage: dr3030 [options]\n\n");

    printf("\nOptions:\n");

    printf("  -m, --mapping   Specify midi key mappings\n");
    printf("  -l, --list-midi List all available midi clients\n");
    printf("  -z, --debug     Turn on debug mode\n");
    printf("\n");

    printf("  -h, --help     Show application usage\n");
    printf("  -v, --version  Print version\n\n");
}

void print_version() {
    printf("DR3030 v%s.%s.%s\nrevision: %s\nbuild date: %s\n\n",
           DR_VERSION_MAJOR, DR_VERSION_MINOR, DR_VERSION_PATCH,
           DR_VERSION_BUILD, DR_VERSION_DATE);

    printf("Authors: Bailey Cosier <bailey@cosier.ca>\n");
    printf("Homepage: https://github.com/cosier/dr3030\n\n");
}

int requires_target_specified(char* cmd) {
    printf("--%s command requires a target to be specified.\n", cmd);
    printf("Use the -t,--target=<id:port> argument to specify a target\n\n");
    print_usage();

    exit(EXIT_FAILURE);
}

int requires_source_specified(char* cmd) {
    printf("--%s command requires a Midi Client Source to be specified.\n",
           cmd);
    printf(
        "Use the -s,--source=<id:port> argument to specify a Midi source\n\n");
    print_usage();

    exit(EXIT_FAILURE);
}

bool verify_valid_midi_client(char* client) {
    if (strstr(client, ":") == NULL) {
        printf("Midi Client (%s) does not contain port specification \":\".\n",
               client);

        printf("For example, here is a valid Midi Client: \"123:0\"\n\n");
        return false;
    }

    if (!dm_client_exists(client)) {
        printf("Midi Client (%s) was not found or is no longer available.\n\n",
               client);
        printf("Try \"dr3030 --list-midi\" to list available Midi Clients.\n");
        printf("The [xx:x] denotes the client id + port number.\n\n");
        return false;
    }

    return true;
}

void handle_invalid_send_args(int argc, int optind) {
    fprintf(stderr, "Send requires 4 parameters: (note) (ch) "
                    "(velocity) (on/off)\nargc/optind [%d/%d] \n\n",
            argc, optind);
    print_usage();
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
    int opt = 0;

    bool list = false, version = false, help = false, monitor = false,
         debug = false;

    char* target = NULL;
    char* source = NULL;
    char* mapsrc = NULL;
    char* cmds = NULL;

    dm_mapping* mapping = NULL;

    int keys = 49;

    // Specifying the expected options
    // The two options l and b expect numbers as argument
    static struct option long_options[] = {
        {"list-midi", no_argument, 0, 'l'},
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {"debug", no_argument, 0, 'z'},
        {"mapping", no_argument, 0, 'm'},
        {"keys", required_argument, 0, 'k'},
        {"target", required_argument, 0, 't'},
        {"source", required_argument, 0, 'x'},
        {0, 0, 0, 0}};

    int long_index = 0;
    while ((opt = getopt_long(argc, argv, "lhvzmr:t:k:x:", long_options,
                              &long_index)) != -1) {

        switch (opt) {
        case 'l':
            list = true;
            break;

        case 'v':
            version = true;
            break;

        case 'h':
            help = true;
            break;

        case 'z':
            debug = true;
            break;

        case 'k':
            keys = atoi(optarg);
            break;

        case 't':
            target = optarg;
            break;

        case 'x':
            source = optarg;
            break;

        case 'm':
            mapsrc = optarg;
            break;

        default:
            print_usage();
            return 1;
        }
    }

    // Capture n amount of mappings provided to the CLI
    if (optind < argc) {
        cmds = calloc(sizeof(char*) * 1024, sizeof(char*));
        char* cat_ptr = cmds;

        int opts = argc - optind + 1;
        int base = optind;
        for (int i = 0; i < opts; ++i) {
            char* arg_str = argv[base + i];
            if (arg_str == NULL) {
                break;
            }

            util_cat(&cat_ptr, arg_str);
            if (i < (opts - 1)) {
                util_cat(&cat_ptr, ", ");
            }
        }
    }

    // printf("mapsrc: %s\noptind: %d/%d\n", mapsrc, optind, argc);

    // Flip global debug flag.
    if (debug) {
        dm_driver_debug();
        util_debug("\033c[dimappio started]: %lu\n", util_micros());
    }

    // Version output and then exit.
    if (version) {
        print_version();
        return 0;
    }

    // Show cli help usage and exit.
    if (help) {
        print_usage();
        return 0;
    }

    // List available midi client ports then exit.
    if (list) {
        dm_list_clients();
        return 0;
    }

    // If target client:port is specified, verify that it exists.
    if (target != NULL) {
        if (!verify_valid_midi_client(target)) {
            exit(EXIT_FAILURE);
        }
    } else {
        dm_device* midi_through = dm_get_midi_through();
        if (midi_through != NULL) {
            target = malloc(sizeof(char*) * 6);
            sprintf(target, "%d:%d", midi_through->client, midi_through->port);
        }
    }

    // If source client:port is specified, verify that it exists.
    if (source != NULL) {
        if (!verify_valid_midi_client(source)) {
            exit(EXIT_FAILURE);
        }
    }

    // If mappings are provided, check for a valid target and source.
    // Build a MapDefinition struct from cli remap definition.
    if (mapsrc != NULL) {
        if (source == NULL) {
            requires_source_specified("remap");
        } else {
            if (!verify_valid_midi_client(source)) {
                exit(EXIT_FAILURE);
            }
        }

        // Build mappings from a comma delimited string.
        util_debug("main: attempting to build mappings from list using: %s\n",
                 mapsrc);
        mapping = dm_mapping_from_list(mapsrc);
        // free(mapsrc);
    } else {
        util_debug("main: building solo mappings\n");
        mapping = dm_build_mapping();
    }

    if (mapping == NULL) {
        util_error("Invalid Mapping provided\n");
        util_error("-----------------------------------\n");
        print_usage();
        exit(EXIT_FAILURE);
    } else {
        util_debug("main: mappings created successfully\n");
    }

    dm_options* options = dm_create_options();
    options->mapping = mapping;
    options->keys = keys;
    options->source = source;
    options->target = target;

    dr_interface();

    // Engage in monitor loop.
    // Uses a source to poll events from.
    //
    // Sourced events are passed through the remap filter
    // before being broadcast to any subscribers.
    /* if (monitor || mapsrc != NULL) { */

    print_usage();
    return 0;
}
