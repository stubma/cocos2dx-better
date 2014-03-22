/****************************************************************************
 Author: Luma (stubma@gmail.com)
 
 https://github.com/stubma/cocos2dx-better
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "CBTMXLoader.h"
#include "CBTMXMapInfo.h"
#include "CBTMXTileSetInfo.h"
#include "CBTMXObject.h"
#include "CBTMXObjectGroup.h"
#include "CBTMXLayerInfo.h"
#include "CCUtils.h"
#include "CCBase64.h"
#include "support/zip_support/ZipUtils.h"

NS_CC_BEGIN

CBTMXLoader::CBTMXLoader() :
m_compressed(false),
m_lastGid(-1) {
	m_map = CBTMXMapInfo::create();
	CC_SAFE_RETAIN(m_map);
}

CBTMXLoader::~CBTMXLoader() {
	CC_SAFE_RELEASE(m_map);
}

CBTMXLoader* CBTMXLoader::create() {
	CBTMXLoader* l = new CBTMXLoader();
	return (CBTMXLoader*)l->autorelease();
}

CBTMXLoader::TMXTag CBTMXLoader::getTag(const char* name) {
	if(!strcmp(name, "map"))
		return MAP;
	else if(!strcmp(name, "tileset"))
		return TILESET;
	else if(!strcmp(name, "tile"))
		return TILE;
	else if(!strcmp(name, "image"))
		return IMAGE;
	else if(!strcmp(name, "layer"))
		return LAYER;
	else if(!strcmp(name, "data"))
		return DATA;
	else if(!strcmp(name, "properties"))
		return PROPERTIES;
	else if(!strcmp(name, "property"))
		return PROPERTY;
	else if(!strcmp(name, "objectgroup"))
		return OBJECTGROUP;
	else if(!strcmp(name, "object"))
		return OBJECT;
	else if(!strcmp(name, "polyline"))
		return POLYLINE;
	else if(!strcmp(name, "polygon"))
		return POLYGON;
	else
		return UNKNOWN;
}

CBTMXLoader::TMXAttr CBTMXLoader::getAttr(const char* name) {
	if(!strcmp(name, "version"))
		return ATTR_VERSION;
	else if(!strcmp(name, "orientation"))
		return ATTR_ORIENTATION;
	else if(!strcmp(name, "width"))
		return ATTR_WIDTH;
	else if(!strcmp(name, "height"))
		return ATTR_HEIGHT;
	else if(!strcmp(name, "tilewidth"))
		return ATTR_TILEWIDTH;
	else if(!strcmp(name, "tileheight"))
		return ATTR_TILEHEIGHT;
	else if(!strcmp(name, "name"))
		return ATTR_NAME;
	else if(!strcmp(name, "firstgid"))
		return ATTR_FIRSTGID;
	else if(!strcmp(name, "spacing"))
		return ATTR_SPACING;
	else if(!strcmp(name, "margin"))
		return ATTR_MARGIN;
	else if(!strcmp(name, "visible"))
		return ATTR_VISIBLE;
	else if(!strcmp(name, "opacity"))
		return ATTR_OPACITY;
	else if(!strcmp(name, "x"))
		return ATTR_X;
	else if(!strcmp(name, "y"))
		return ATTR_Y;
	else if(!strcmp(name, "type"))
		return ATTR_TYPE;
	else if(!strcmp(name, "color"))
		return ATTR_COLOR;
	else if(!strcmp(name, "points"))
		return ATTR_POINTS;
	else
		return ATTR_UNKNOWN;
}

void CBTMXLoader::pushTag(TMXTag tag) {
	m_tags.push_back(tag);
}

void CBTMXLoader::popTag() {
	m_tags.pop_back();
}

CBTMXLoader::TMXTag CBTMXLoader::topTag() {
	return *m_tags.rbegin();
}

CBTMXLoader::TMXTag CBTMXLoader::parentTag() {
	return *(m_tags.rbegin() + 1);
}

CBTMXLoader::TMXTag CBTMXLoader::grandpaTag() {
	return *(m_tags.rbegin() + 2);
}

const char* CBTMXLoader::valueForKey(const char** atts, const char* name) {
	for(int i = 0; atts[i] != NULL; i += 2) {
		if(!strcmp(atts[i], name))
			return atts[i + 1];
	}
	return NULL;
}

CBTMXMapInfo* CBTMXLoader::load(const string& tmxFile) {
	// get dir
	m_tmxDir = CCUtils::deleteLastPathComponent(tmxFile);
	
	// start
	unsigned long size;
	unsigned char* data = CCFileUtils::sharedFileUtils()->getFileData(tmxFile.c_str(), "rb", &size);
	bool success = load((const char*)data, (int)size);
	free(data);
	return success ? m_map : NULL;
}

CBTMXMapInfo* CBTMXLoader::load(const char* tmxData, int length, const string& resourcePath) {
	// get dir
	m_tmxDir = resourcePath;
	
	// start load
	if(load(tmxData, length)) {
		return m_map;
	} else {
		return NULL;
	}
}

bool CBTMXLoader::load(const char* data, int length) {
	CCSAXParser parser;
    if(!parser.init("UTF-8")) {
        return false;
    }
    parser.setDelegator(this);
    return parser.parse(data, length);
}

void CBTMXLoader::startElement(void* ctx, const char* name, const char** atts) {
	// set tag
	pushTag(getTag((const char*)name));
	
	switch(topTag()) {
		case MAP:
			for(int i = 0; atts[i] != NULL; i += 2) {
				// get attr and value
				const char* key = (const char*)atts[i];
				const char* value = (const char*)atts[i + 1];
				TMXAttr attr = getAttr(key);
				
				// check attr
				switch(attr) {
					case ATTR_VERSION:
						if(strcmp("1.0", value)) {
							CCLOGERROR("Doesn't support TMX file whose version is not 1.0");
						}
						break;
					case ATTR_ORIENTATION:
						if(!strcmp(value, "orthogonal"))
							m_map->setOrientation(kCBTMXOrientationOrthogonal);
						else if(!strcmp(value, "isometric"))
							m_map->setOrientation(kCBTMXOrientationIsometric);
						else if(!strcmp(value, "hexagonal"))
							m_map->setOrientation(kCBTMXOrientationHexagonal);
						else {
							CCLOGERROR("Unsupported orientation: %s", value);
						}
						break;
					case ATTR_WIDTH:
						m_map->setMapWidth(atoi(value));
						break;
					case ATTR_HEIGHT:
						m_map->setMapHeight(atoi(value));
						break;
					case ATTR_TILEWIDTH:
						m_map->setTileWidth(atof(value) / CC_CONTENT_SCALE_FACTOR());
						break;
					case ATTR_TILEHEIGHT:
						m_map->setTileHeight(atof(value) / CC_CONTENT_SCALE_FACTOR());
						break;
					default:
						break;
				}
			}
			break;
		case TILESET:
		{
			const char* externalFile = valueForKey((const char**)atts, "source");
			if(externalFile == NULL) {
				CBTMXTileSetInfo* tileset = CBTMXTileSetInfo::create();
				m_map->getTileSets().addObject(tileset);
				
				for(int i = 0; atts[i] != NULL; i += 2) {
					// get attr and value
					const char* key = (const char*)atts[i];
					const char* value = (const char*)atts[i + 1];
					TMXAttr attr = getAttr(key);
					
					// check attr
					switch(attr) {
						case ATTR_NAME:
							tileset->setName(value);
							break;
						case ATTR_FIRSTGID:
							tileset->setFirstGid(atoi(value));
							break;
						case ATTR_SPACING:
							tileset->setSpacing(atof(value) / CC_CONTENT_SCALE_FACTOR());
							break;
						case ATTR_MARGIN:
							tileset->setMargin(atof(value) / CC_CONTENT_SCALE_FACTOR());
							break;
						case ATTR_TILEWIDTH:
							tileset->setTileWidth(atof(value) / CC_CONTENT_SCALE_FACTOR());
							break;
						case ATTR_TILEHEIGHT:
							tileset->setTileHeight(atof(value) / CC_CONTENT_SCALE_FACTOR());
							break;
						default:
							break;
					}
				}
			} else {
				string externalFilePath = CCUtils::appendPathComponent(m_tmxDir, externalFile);
				unsigned long size;
				unsigned char* data = CCFileUtils::sharedFileUtils()->getFileData(externalFilePath.c_str(), "rb", &size);
				bool success = load((const char*)data, (int)size);
				free(data);
				
				// firstgid is not written in external tileset, so we must read it from current tmx
				if(success) {
					CBTMXTileSetInfo* tileset = (CBTMXTileSetInfo*)m_map->getTileSets().lastObject();
					for(int i = 0; atts[i] != NULL; i += 2) {
						// get attr and value
						const char* key = (const char*)atts[i];
						const char* value = (const char*)atts[i + 1];
						TMXAttr attr = getAttr(key);
						
						// check attr
						switch(attr) {
							case ATTR_FIRSTGID:
								tileset->setFirstGid(atoi(value));
								break;
							default:
								break;
						}
					}
				}
			}
			break;
		}
		case IMAGE:
		{
			string imageName = CCUtils::lastPathComponent(valueForKey((const char**)atts, "source"));
			string imagePath = CCUtils::appendPathComponent(m_tmxDir, imageName);
			CBTMXTileSetInfo* tileset = (CBTMXTileSetInfo*)m_map->getTileSets().lastObject();
			tileset->setSourceImagePath(imagePath);
			break;
		}
		case TILE:
		{
			CBTMXTileSetInfo* tileset = (CBTMXTileSetInfo*)m_map->getTileSets().lastObject();
			m_lastGid = tileset->getFirstGid() + atoi(valueForKey((const char**)atts, "id"));
			break;
		}
		case PROPERTY:
			switch(grandpaTag()) {
				case MAP:
					m_map->addProperty(valueForKey((const char**)atts, "name"), (const char*)valueForKey((const char**)atts, "value"));
					break;
				case TILE:
					m_map->addTileProperty(m_lastGid, valueForKey((const char**)atts, "name"), (const char*)valueForKey((const char**)atts, "value"));
					break;
				case LAYER:
				{
					CBTMXLayerInfo* layer = (CBTMXLayerInfo*)m_map->getLayers().lastObject();
					layer->addProperty(valueForKey((const char**)atts, "name"), valueForKey((const char**)atts, "value"));
					break;
				}
				case OBJECTGROUP:
				{
					CBTMXObjectGroup* group = (CBTMXObjectGroup*)m_map->getObjectGroups().lastObject();
					group->addProperty(valueForKey((const char**)atts, "name"), valueForKey((const char**)atts, "value"));
					break;
				}
				case OBJECT:
				{
					CBTMXObjectGroup* group = (CBTMXObjectGroup*)m_map->getObjectGroups().lastObject();
					CBTMXObject* object = group->getObjectAt(group->getObjectCount() - 1);
					object->addProperty(valueForKey((const char**)atts, "name"), valueForKey((const char**)atts, "value"));
					break;
				}
				default:
					break;
			}
			break;
		case LAYER:
		{
			CBTMXLayerInfo* layer = CBTMXLayerInfo::create();
			m_map->getLayers().addObject(layer);
			
			for(int i = 0; atts[i] != NULL; i += 2) {
				// get attr and value
				const char* key = (const char*)atts[i];
				const char* value = (const char*)atts[i + 1];
				TMXAttr attr = getAttr(key);
				
				// check some optional attribute
				const char* visible = valueForKey((const char**)atts, "visible");
				if(visible == NULL)
					layer->setVisible(true);
				const char* alpha = valueForKey((const char**)atts, "opacity");
				if(alpha == NULL)
					layer->setAlpha(255);
				
				// check attr
				switch(attr) {
					case ATTR_NAME:
						layer->setName(value);
						break;
					case ATTR_WIDTH:
						layer->setLayerWidth(atoi(value));
						break;
					case ATTR_HEIGHT:
						layer->setLayerHeight(atoi(value));
						break;
					case ATTR_VISIBLE:
						layer->setVisible(atoi(value) == 1);
						break;
					case ATTR_OPACITY:
						layer->setAlpha(atof(value) * 255.f);
						break;
					case ATTR_X:
						layer->setOffsetX(atof(value) / CC_CONTENT_SCALE_FACTOR());
						break;
					case ATTR_Y:
						layer->setOffsetY(atof(value) / CC_CONTENT_SCALE_FACTOR());
						break;
					default:
						break;
				}
			}
			break;
		}
		case DATA:
		{
			// check encoding
			const char* encoding = valueForKey((const char**)atts, "encoding");
			if(strcmp(encoding, "base64")) {
				CCLOGERROR("Unsupported layer data encoding: %s", encoding);
				break;
			}
			
			// check compression
			const char* compression = valueForKey((const char**)(atts), "compression");
			m_compressed = compression != NULL;
			if(compression != NULL && strcmp(compression, "gzip") && strcmp(compression, "zlib")) {
				CCLOGERROR("Unsupported layer data compression: %s", compression);
				break;
			}
			
			break;
		}
		case OBJECTGROUP:
		{
			CBTMXObjectGroup* group = CBTMXObjectGroup::create();
			m_map->getObjectGroups().addObject(group);
			
			for(int i = 0; atts[i] != NULL; i += 2) {
				// get attr and value
				const char* key = (const char*)atts[i];
				const char* value = (const char*)atts[i + 1];
				TMXAttr attr = getAttr(key);
				
				// check attr
				switch(attr) {
					case ATTR_NAME:
						group->setName(value);
						break;
					case ATTR_X:
						group->setOffsetX(atof(value) / CC_CONTENT_SCALE_FACTOR());
						break;
					case ATTR_Y:
						group->setOffsetY(atof(value) / CC_CONTENT_SCALE_FACTOR());
						break;
					case ATTR_COLOR:
					{
						int color;
						sscanf(value, "#%x", &color);
						color |= 0xff000000;
						group->setColor(color);
						break;
					}
					case ATTR_OPACITY:
						group->setOpacity(atof(value));
						break;
					default:
						break;
				}
			}
			
			break;
		}
		case OBJECT:
		{
			// create new object hash
			CBTMXObjectGroup* group = (CBTMXObjectGroup*)m_map->getObjectGroups().lastObject();
			CBTMXObject* object = group->newObject();
			
			// set object property
			for(int i = 0; atts[i] != NULL; i += 2) {
				// get attr and value
				const char* key = (const char*)atts[i];
				const char* value = (const char*)atts[i + 1];
				TMXAttr attr = getAttr(key);
				
				// check attr
				switch(attr) {
					case ATTR_NAME:
						object->setName(value);
						break;
					case ATTR_TYPE:
						object->setType(value);
						break;
					case ATTR_X:
					{
						float x = atof(value) / CC_CONTENT_SCALE_FACTOR();
						object->getPosition().x = x;
						break;
					}
					case ATTR_Y:
					{
						float y = atof(value) / CC_CONTENT_SCALE_FACTOR();
						object->getPosition().y = y;
						break;
					}
					case ATTR_WIDTH:
					{
						float w = atof(value) / CC_CONTENT_SCALE_FACTOR();
						object->getSize().width = w;
						break;
					}
					case ATTR_HEIGHT:
					{
						float h = atof(value) / CC_CONTENT_SCALE_FACTOR();
						object->getSize().height = h;
						break;
					}
					default:
						break;
				}
			}
			break;
		}
		case POLYGON:
		case POLYLINE:
		{
			switch(parentTag()) {
				case OBJECT:
				{
					CBTMXObjectGroup* group = (CBTMXObjectGroup*)m_map->getObjectGroups().lastObject();
					CBTMXObject* object = group->getObjectAt(group->getObjectCount() - 1);
					
					// set type
					switch(topTag()) {
						case POLYGON:
							object->setShape(CBTMXObject::POLYGON);
							break;
						case POLYLINE:
							object->setShape(CBTMXObject::POLYLINE);
							break;
						default:
							break;
					}
					
					// parse attributes
					for(int i = 0; atts[i] != NULL; i += 2) {
						// get attr and value
						const char* key = (const char*)atts[i];
						const char* value = (const char*)atts[i + 1];
						TMXAttr attr = getAttr(key);
						
						// check attr
						switch(attr) {
							case ATTR_POINTS:
							{
								// parse space character
								char* p = strtok((char*)value, " ");
								char* n = NULL;
								while(p) {
									n = strtok(NULL, " ");
									
									// parse point
									float x, y;
									sscanf(p, "%f,%f", &x, &y);
									object->getPoints().addPoint(x / CC_CONTENT_SCALE_FACTOR(), y / CC_CONTENT_SCALE_FACTOR());
									
									// next token
									p = n;
								}
								
								break;
							}
							default:
								break;
						}
					}
					
					break;
				}
				default:
					break;
			}
			break;
		}
		default:
			break;
	}
}

void CBTMXLoader::endElement(void* ctx, const char* name) {
	// pop
	popTag();
}

void CBTMXLoader::textHandler(void* ctx, const char* s, int len) {
	// check tag
	switch(topTag()) {
		case DATA:
		{
			// decode
			char* tmp = (char*)calloc(len + 1, sizeof(char));
			strncpy(tmp, (const char*)s, len);
			string tmpS = tmp;
			tmpS = CCUtils::trim(tmpS);
			char* data = (char*)CCBase64::decode(tmpS, &len);
			free(tmp);
			
			// decompress
			if(m_compressed) {
				unsigned char* deflated = NULL;
				CBTMXLayerInfo* layer = (CBTMXLayerInfo*)m_map->getLayers().lastObject();
				int sizeHint = (int)(layer->getLayerWidth() * layer->getLayerHeight() * sizeof(unsigned int));
				len = ZipUtils::ccInflateMemoryWithHint((unsigned char*)data, (unsigned int)len, &deflated, sizeHint);
				free(data);
				data = (char*)deflated;
			}
			
			// convert to int array
			if(len % 4 == 0) {
				CBTMXLayerInfo* layer = (CBTMXLayerInfo*)m_map->getLayers().lastObject();
				layer->setTiles((int*)malloc(len / 4 * sizeof(int)));
				int* tiles = layer->getTiles();
				for(int i = 0; i < len; i += 4) {
					tiles[i / 4] = ((data[i + 3] & 0xFF) << 24) | ((data[i + 2] & 0xFF) << 16) | ((data[i + 1] & 0xFF) << 8) | (data[i] & 0xFF);
				}
			}
			
			free(data);
			break;
		}
		default:
			break;
	}
}

NS_CC_END