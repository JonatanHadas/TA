CC = g++
DEL = rm -f

CMP_FLAGS = -I"C:\MinGW\include\SDL2" -I"enet\include" -g
LNK_FLAGS = -L"C:\MinGW\lib" -L"enet" -lmingw32 -lSDL2main -lSDL2 -lenet64 -lws2_32 -lwinmm

# data_objects
DATA_OBJECT_HEADS = data_objects/serialization
HEADS_data_objects/game_state := data_objects/game_state $(DATA_OBJECT_HEADS)
HEADS_data_objects/game_settings := data_objects/game_settings $(DATA_OBJECT_HEADS)
HEADS_data_objects/game_score := data_objects/game_score $(DATA_OBJECT_HEADS)
HEADS_data_objects/board_geometry := data_objects/board_geometry $(DATA_OBJECT_HEADS)
HEADS_data_objects/drawing_data := data_objects/drawing_data $(HEADS_data_objects/board_geometry)
DATA_OBJECT_HEADS += data_objects/game_state data_objects/game_settings data_objects/game_score data_objects/board_geometry data_objects/drawing_data

COMMON_OBJECTS := data_objects/game_state data_objects/game_settings data_objects/game_score data_objects/board_geometry data_objects/drawing_data

# algorithms
HEADS_algorithms/disjoint_sets := algorithms/disjoint_sets
HEADS_algorithms/subset := algorithms/subset
HEADS_alogoritms/tree_span := algorithms/tree_span algorithms/tree_span_internal algorithms/subset

COMMON_OBJECTS += algorithms/disjoint_sets
SERVER_OBJECTS := algorithms/subset algorithms/tree_span

# utils
HEADS_utils/geometry = utils/geometry

CLIENT_OBJECTS := utils/geometry

# game
GAME_INTERFACE_HEADS := game/game_observer game/player_interface $(DATA_OBJECT_HEADS)
HEADS_game/game_round := game/game_round $(GAME_INTERFACE_HEADS) algorithms/disjoint_sets
HEADS_game/game := game/game $(HEADS_game/game_round) algorithms/tree_span

GAME_HEADS := game/game game/game_round $(GAME_INTERFACE_HEADS) algorithms/disjoint_sets
SERVER_OBJECTS += game/game_round game/game

# gui
HEADS_gui/gui_utils := gui/gui_utils
HEADS_gui/game_drawer := gui/game_drawer $(GAME_INTERFACE_HEADS) gui/gui_utils gui/color utils/geometry algorithms/disjoint_sets
HEADS_gui/game_gui := gui/game_gui gui/game_drawer $(GAME_INTERFACE_HEADS) utils/geometry

GUI_HEADS := gui/game_gui gui/game_drawer $(GAME_INTERFACE_HEADS)
CLIENT_OBJECTS += gui/gui_utils gui/game_drawer gui/game_gui

# net
HEADS_net/enet_wrapper := net/enet_wrapper
HEADS_net/client := net/client net/enet_wrapper net/network_consts
HEADS_net/server := net/server net/enet_wrapper net/network_consts

CLIENT_HEADS := net/client net/enet_wrapper net/network_consts
SERVER_HEADS := net/server net/enet_wrapper net/network_consts
COMMON_OBJECTS += net/enet_wrapper
CLIENT_OBJECTS += net/client
SERVER_OBJECTS += net/server

# net game
HEADS_net_game/game_client := net_game/game_client net_game/game_protocol $(CLIENT_HEADS) $(GAME_INTERFACE_HEADS)
HEADS_net_game/game_server := net_game/game_server net_game/game_protocol $(SERVER_HEADS) $(GAME_HEADS)

CLIENT_HEADS += net_game/game_client
SERVER_HEADS += net_game/game_server

CLIENT_OBJECTS += net_game/game_client
SERVER_OBJECTS += net_game/game_server

# main
HEADS_client_main = $(CLIENT_HEADS) $(GUI_HEADS)
HEADS_server_main = $(SERVER_HEADS) $(GAME_HEADS)

CLIENT_OBJECTS += client_main
SERVER_OBJECTS += server_main

# executables
OBJECTS_game_client := $(COMMON_OBJECTS) $(CLIENT_OBJECTS)
OBJECTS_game_server := $(COMMON_OBJECTS) $(SERVER_OBJECTS)

EXECUTABLES := game_client game_server

# rules
OBJECTS = $(COMMON_OBJECTS) $(SERVER_OBJECTS) $(CLIENT_OBJECTS)

OBJECTS := $(addprefix build/,$(addsuffix .o,$(OBJECTS)))
EXECUTABLES := $(addprefix build/,$(addsuffix .exe,$(EXECUTABLES)))

all: $(EXECUTABLES)

clear:
	$(DEL) $(OBJECTS)

clear_all: clear
	$(DEL) $(EXECUTABLES)

.SECONDEXPANSION:
$(EXECUTABLES): build/%.exe: $$(addprefix build/,$$(addsuffix .o,$$(OBJECTS_$$*)))
	$(CC) $(CMP_FLAGS) $^ -o $@ $(LNK_FLAGS)
	
$(OBJECTS): build/%.o: src/%.cpp $$(addprefix src/,$$(addsuffix .h,$$(HEADS_$$*)))
	$(CC) $(CMP_FLAGS) -c $< -o $@
