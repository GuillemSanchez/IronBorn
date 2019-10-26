#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Window.h"
#include "j1Collisions.h"
#include "j1Scene.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;
	bool is_in;

	p2List_item<MapLayer*>* layer;

	for (layer = data.layers.start; layer; layer = layer->next) {
		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->data->Get(x, y);
				if (tile_id > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tile_id);
					if (tileset != nullptr)
					{
						SDL_Rect r = tileset->GetTileRect(tile_id);
						iPoint pos = MapToWorld(x, y);
						iPoint dim;
						dim.x = r.w;
						dim.y = r.h;
						is_in = IsInCamera(pos, dim, layer->data->properties.speed);
						if (layer->data->properties.draw == 1 && is_in)
							App->render->Blit(tileset->texture, pos.x, pos.y, &r, layer->data->properties.speed);
					}
				}
			}
		}
	}
	
	
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	
	p2List_item<TileSet*>* tile;
	

	for (tile = data.tilesets.start; tile; tile = tile->next) {
		if (tile->next == NULL) {
			break;
		}
		if (tile->next->data->firstgid > id) 
		{
			break;
		}
	}


	return tile->data;
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if(data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tile_width;
		ret.y = y * data.tile_height;
	}
	else if(data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tile_width * 0.5f);
		ret.y = (x + y) * (data.tile_height * 0.5f);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0,0);

	if(data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tile_width;
		ret.y = y / data.tile_height;
	}
	else if(data.type == MAPTYPE_ISOMETRIC)
	{
		
		float half_width = data.tile_width * 0.5f;
		float half_height = data.tile_height * 0.5f;
		ret.x = int( (x / half_width + y / half_height) / 2);
		ret.y = int( (y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

bool j1Map::ChargeColliders()
{
	bool ret = true;
	p2List_item<Objects*>* item;
	item = data.objects.start;
	p2SString WALL, DEATH, WIN;
	WALL.create("WALL");
	DEATH.create("DEATH");
	WIN.create("WIN");

	while (item != NULL)
	{
		p2List_item<SemiCollider*>* collo;
		collo = item->data->kolos.start;
		while (collo != NULL) 
		{
			if (collo->data->type == WALL)
				App->collisions->AddCollider(collo->data->phisbody, COLLIDER_WALL, this);
			if (collo->data->type == DEATH)
				App->collisions->AddCollider(collo->data->phisbody, COLLIDER_DEATH, App->scene); // we will handle the player death on scene
			if (collo->data->type == WIN)
				App->collisions->AddCollider(collo->data->phisbody, COLLIDER_WIN, App->scene);
			collo = collo->next;
		}
		item = item->next;
	}


	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		App->tex->UnLoad(item->data->texture); // Por favor cambiar esto xd.
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// Remove all layers
	p2List_item<MapLayer*>* item2;
	item2 = data.layers.start;

	while(item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	p2List_item<Objects*>* item3;
	item3 = data.objects.start;

	p2List_item<SemiCollider*>* semi_item;


	while (item != NULL)
	{
		p2List_item<SemiCollider*>* semi_item;
		semi_item = item3->data->kolos.start;

		while (semi_item != NULL)
		{
			RELEASE(semi_item->data);
			semi_item = semi_item->next;
		}
		item3->data->kolos.clear();

		RELEASE(item3->data);
		item3 = item3->next;
	}
	data.objects.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;

	p2SString tmp("%s%s",folder.GetString(), file_name);
	
	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;
	for(layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer();

		ret = LoadLayer(layer, lay);

		if(ret == true)
			data.layers.add(lay);
	}
	
	//load objects (Colliders by now) -------------------------------
	pugi::xml_node object;
	for (object = map_file.child("map").child("objectgroup"); object && ret; object = object.next_sibling("objectgroup"))
	{
		Objects* ob = new Objects();

		ret = LoadObjects(object, ob);
		if (ret == true)
			data.objects.add(ob);
		//By now we have all the colliders updated of the map

	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.real_height = data.height * data.tile_height;
		data.real_width = data.width * data.tile_width;

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();
	LoadProperties(node, layer->properties);
	pugi::xml_node layer_data = node.child("data");

	if(layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[layer->width*layer->height];
		memset(layer->data, 0, layer->width*layer->height);

		int i = 0;
		for(pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
		{
			layer->data[i++] = tile.attribute("gid").as_int(0);
		}
	}

	return ret;
}

// Load a group of properties from a node and fill a list with it
bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = true;
	bool test = false;
	p2SString help;
	p2SString fl;
	fl.create("float");

	for (pugi::xml_node data = node.child("properties").child("property"); data; data = data.next_sibling("property")) {
		help.create(data.attribute("type").as_string());

		if (help == fl)
			properties.speed = data.attribute("value").as_float();
		else
			properties.draw = data.attribute("value").as_int();

	}
	if (properties.speed == NULL) {
		properties.speed = 1.0f;
	}
	if (properties.draw == NULL) {
		properties.draw = 1;
	}
	return ret;
}

//Load colliders from Tile
bool j1Map::LoadObjects(pugi::xml_node& node, Objects* objects)
{
	bool ret = true;

	objects->name.create(node.attribute("name").as_string());

	bool ret_2 = true;
	pugi::xml_node coll;
	for (coll = node.child("object"); coll && ret_2; coll = coll.next_sibling("object"))
	{
		SemiCollider* co = new SemiCollider();

		//We will use the type to classify the collider type

		co->type.create(coll.attribute("type").as_string());
		co->phisbody.x = coll.attribute("x").as_int();
		co->phisbody.y = coll.attribute("y").as_int();
		co->phisbody.w = coll.attribute("width").as_int();
		co->phisbody.h = coll.attribute("height").as_int();


		if (coll == NULL) {
			ret_2 = false;
			RELEASE(co);
		}
		objects->kolos.add(co);
	}
	
	if (objects == NULL) {
		ret = false;
		RELEASE(objects);
	}


	return ret;
}

bool j1Map::IsInCamera(iPoint pos, iPoint dim, float speed)
{
	bool ret = false;
	int scale = App->win->GetScale();
	SDL_Rect helper;

	helper.x = (int)(App->render->camera.x * speed) + pos.x * scale;
	helper.y = (int)(App->render->camera.y * speed) + pos.y * scale;
	helper.w = dim.x * scale;
	helper.h = dim.y * scale;


	// the think is that the sum over this lane is a sum to see if the tile is betwen the size of the camera, so if the tile is in camera
	// the number in x and y is betwen 0 and w/h of camera. 

	if (helper.x + helper.w >= 0)  { //left
		if (helper.y <= App->render->camera.h) { //down
			if (helper.x <= App->render->camera.w) { //right
				if ((helper.y + helper.h) >= 0) { //up
					ret = true;
				}

			}
		}
	}
	
	return ret;
}
