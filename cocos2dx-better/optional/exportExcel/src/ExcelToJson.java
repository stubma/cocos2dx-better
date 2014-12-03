import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Vector;

import org.apache.poi.hssf.usermodel.HSSFWorkbook;
import org.apache.poi.ss.usermodel.Cell;
import org.apache.poi.ss.usermodel.Row;
import org.apache.poi.ss.usermodel.Sheet;
import org.apache.poi.ss.usermodel.Workbook;
import org.apache.poi.xssf.usermodel.XSSFWorkbook;

import com.alibaba.fastjson.JSONObject;

public class ExcelToJson {
	public void start(File[] files) {
		try {
	        int len = files.length;
	        for (int i = 0; i < len; i++) {
	        	String fileName = files[i].getName();
	        	System.out.println("exporting:" + fileName + "---" + (i + 1) + "/" + len);
	        	Workbook book = getWorkbook(files[i]);
	        	getExcel(book, files[i]);
	        }
        } catch (IOException e) {
        }
		System.out.println("done");
	}
	
	private Workbook getWorkbook(File f) {
		Workbook book = null;
		try {
	        String fileName = f.getName();
	        if (f.getName().endsWith(".xls")) {
	        	book = new HSSFWorkbook(new FileInputStream(f));
	        }
	        if (fileName.endsWith(".xlsx")) {
	        	book = new XSSFWorkbook(new FileInputStream(f));
	        }
        } catch (Exception e) {
        }
		
		return book;
	}
	
	int getSheetCount(Workbook book) {
		int i = 0;
		while(true) {
			try {
	            if(book.getSheetAt(i) == null) {
	            	return i;
	            } else {
	            	i++;
	            }
            } catch (IllegalArgumentException e) {
            	return i;
            }
		}
	}
	
	private String firstCapital(String s) {
		return s.substring(0, 1).toUpperCase() + s.substring(1);
	}
	
	private String firstLowercase(String s) {
		return s.substring(0, 1).toLowerCase() + s.substring(1);
	}

	private void getExcel(Workbook book, File file) throws IOException {
		JSONObject json = new JSONObject();
		
		// now, just convert first sheet
		for(int s = 0; s < 1; s++) {
			Sheet cSheet = book.getSheetAt(s);
			
			// row num
			int rowNum = cSheet.getLastRowNum();

			// field and type row
			Row fieldRow = cSheet.getRow(2);
			Row typeRow = cSheet.getRow(3);

			String idColName = fieldRow.getCell(0).getStringCellValue();
			idColName = firstCapital(idColName);
			for (int i = 4; i <= rowNum; i++) {
				// json of this row
				JSONObject rowJson = new JSONObject();
				
				// if cell is empty, skip
				int colNum = fieldRow.getLastCellNum();
				Row row = cSheet.getRow(i);
				if(row == null)
					continue;
				Cell cell0 = row.getCell(0);
				if (cell0 == null)
					continue;
				
				// put row
				// if not numeric and value is empty, skip
				if (row.getCell(0).getCellType() == Cell.CELL_TYPE_NUMERIC) {
					rowJson.put(idColName + "", row.getCell(0).getNumericCellValue());
					json.put("" + rowJson.getIntValue(idColName), rowJson);
				} else {
					String sValue = row.getCell(0).getStringCellValue().trim();
					if("".equals(sValue))
						continue;
					rowJson.put(idColName, sValue);
					json.put(rowJson.getString(idColName), rowJson);
				}

				for (int j = 1; j < colNum; j++) {
					if (fieldRow.getCell(j) == null || fieldRow.getCell(j).getCellType() == Cell.CELL_TYPE_BLANK)
						continue;
					
					// col name and quick reject empty col
					String colName = fieldRow.getCell(j).getStringCellValue();
					if (colName == null || "".equals(colName))
						continue;
					colName = firstCapital(colName);
					
					// get type and quick reject empty type
					String type = typeRow.getCell(j).getStringCellValue();
					if (type == null || "".equals(type))
						continue;
					
					// get cell and check its type
					Cell cell = row.getCell(j);
					if (cell == null || cell.getCellType() == Cell.CELL_TYPE_BLANK) {
						if (type.equalsIgnoreCase("int") || type.equalsIgnoreCase("Byte") || type.equalsIgnoreCase("float") || type.equalsIgnoreCase("bool")) {
							rowJson.put(colName, 0);
						} else {
							rowJson.put(colName, "");
						}
					} else if (cell.getCellType() == Cell.CELL_TYPE_STRING) {
						if(type.equalsIgnoreCase("luafunc")) {
							String lua = cell.getStringCellValue();
							lua = lua.replace("\n", "\\n");
							lua = lua.replace("\r", "\\r");
							rowJson.put(colName, lua);
						} else {
							rowJson.put(colName, cell.getStringCellValue());
						}
					} else if (cell.getCellType() == Cell.CELL_TYPE_NUMERIC) {
						rowJson.put(colName, cell.getNumericCellValue());
					} else if(cell.getCellType() == Cell.CELL_TYPE_BOOLEAN) {
						rowJson.put(colName, cell.getBooleanCellValue() ? "true" : "false");
					} else { // 公式
						rowJson.put(colName, cell.getStringCellValue());
					}
				}
			}
			
			// generate json
			String className = "X" + firstCapital(book.getSheetName(0));
			String jsonPath = new File(file.getParentFile(), className + ".json").getAbsolutePath();
			writeFile(jsonPath, json.toJSONString());
			
			// generate c++
			generateCpp(file, className, cSheet);
		}
	}

	public void writeFile(String filePath, String sets) throws IOException {
		BufferedOutputStream buff = null;
		FileOutputStream outStream = null;
		try {
			outStream = new FileOutputStream(new File(filePath));
			buff = new BufferedOutputStream(outStream);
			buff.write(sets.getBytes("utf-8"));
			buff.flush();
			buff.close();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				outStream.close();
				buff.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	private void generateCpp(File xlsFile, String className, Sheet sheet) {
		File outputDir = xlsFile.getParentFile();

		// name and type of id column
		Row fieldRow = sheet.getRow(2);
		Row typeRow = sheet.getRow(3);
		String idName = fieldRow.getCell(0).getStringCellValue();
		String idType = typeRow.getCell(0).getStringCellValue();
		boolean idIsString = idType.equalsIgnoreCase("string");
		
		StringBuilder hfile = new StringBuilder();
		hfile.append("// Auto generated by exportExcel, don't modify it\n")
			.append("#ifndef __" + className + "__\n")
			.append("#define __" + className + "__\n")
			.append("\n")
			.append("#include \"QYJson.h\"\n\n")
			.append("class " + className + " : public CCObject {\n")
			.append("protected:\n")
			.append("\t" + className + "();\n")
			.append("\n")
			.append("\tstatic void ensureLoaded();\n")
			.append("\n")
			.append("public:\n")
			.append("\tvirtual ~" + className + "();\n")
			.append("\tstatic " + className + "* create(" + (idIsString ? "const string&" : "int") + " key);\n")
			.append("\tstatic " + className + "* createWithIndex(int index);\n")
			.append("\tstatic " + className + "* createEmpty(" + (idIsString ? "const string&" : "int") + " _id);\n")
			.append("\tstatic int count();\n")
			.append("\tstatic Json::Value& query(const string& path);\n")
			.append("\tstatic int indexOf(" + (idIsString ? "const string&" : "int") + " _id);\n")
			.append("\tstatic int indexOf(" + className + "* t);\n")
			.append("\n")
			.append("\tvirtual bool initWithIndex(int index);\n")
			.append("\tvirtual bool initWithKey(" + (idIsString ? "const string&" : "int") + " key);\n")
			.append("\tvirtual bool initWithValue(const Json::Value& item);\n")
			.append("\tvirtual bool initWithId(" + (idIsString ? "const string&" : "int") + " _id);\n")
			.append("\tJson::Value toValue();\n")
			.append("\n");
		
		int len = sheet.getRow(2).getLastCellNum();
		for (int i = 0; i < len; i++) {
			if (sheet.getRow(2).getCell(i) == null || sheet.getRow(2).getCell(i).getStringCellValue().equals(""))
				continue;
			String field = sheet.getRow(2).getCell(i).getStringCellValue();
			if (sheet.getRow(0).getCell(i).getCellComment() != null
			        && !sheet.getRow(0).getCell(i).getCellComment().getString().equals("")) {
				hfile.append("\t/*" + sheet.getRow(0).getCell(i).getCellComment().getString() + "*/\n");
			}
			
			String type = sheet.getRow(3).getCell(i).getStringCellValue();
			if (type.equalsIgnoreCase("Byte") || type.equalsIgnoreCase("Int")) {
				hfile.append("\tCC_SYNTHESIZE(int, m_" + firstLowercase(field) + ", " + firstCapital(field) + ");\n");
			} else if (type.equalsIgnoreCase("bool")) {
				hfile.append("\tCC_SYNTHESIZE_BOOL(m_" + firstLowercase(field) + ", " + firstCapital(field) + ");\n");
			} else if (type.equalsIgnoreCase("Float")) {
				hfile.append("\tCC_SYNTHESIZE(float, m_" + firstLowercase(field) + ", " + firstCapital(field) + ");\n");
			} else if (type.equalsIgnoreCase("String") || type.equalsIgnoreCase("luafunc")) {
				hfile.append("\tCC_SYNTHESIZE_PASS_BY_REF(string, m_" + firstLowercase(field) + ", " + firstCapital(field) + ");\n");
			} else if(type.equalsIgnoreCase("StringArray")) {
				hfile.append("\tCC_SYNTHESIZE_PASS_BY_REF(CCArray, m_" + firstLowercase(field) + ", " + firstCapital(field) + ");\n");
				hfile.append("\tstring get" + firstCapital(field) + "At(int index);\n");
			} else if(type.equalsIgnoreCase("IntArray")) {
				hfile.append("\tCC_SYNTHESIZE_PASS_BY_REF(CCArray, m_" + firstLowercase(field) + ", " + firstCapital(field) + ");\n");
				hfile.append("\tint get" + firstCapital(field) + "At(int index);\n");
			} else if(type.equalsIgnoreCase("FloatArray")) {
				hfile.append("\tCC_SYNTHESIZE_PASS_BY_REF(CCArray, m_" + firstLowercase(field) + ", " + firstCapital(field) + ");\n");
				hfile.append("\tfloat get" + firstCapital(field) + "At(int index);\n");
			} else if(type.equalsIgnoreCase("BoolArray")) {
				hfile.append("\tCC_SYNTHESIZE_PASS_BY_REF(CCArray, m_" + firstLowercase(field) + ", " + firstCapital(field) + ");\n");
				hfile.append("\tbool get" + firstCapital(field) + "At(int index);\n");
			}
		}
		
		hfile.append("};\n");
		hfile.append("#endif // defined(__" + className +  "__)\n");
		try {
			File dstFile = new File(outputDir, className + ".h");
			writeFile(dstFile.getAbsolutePath(), hfile.toString());
		} catch (IOException e) {
			e.printStackTrace();
		}

		// cfile
		StringBuilder cfile = new StringBuilder()
			.append("#include \"" + className + ".h\"\n")
			.append("\n")
			.append("static Json::Value sJSON;\n")
			.append("static bool sLoaded = false;\n")
			.append("\n")
			.append(className + "::" + className + "() {\n")
			.append("}\n")
			.append("\n")
			.append(className + "::~" + className + "() {\n")
			.append("}\n")
			.append("\n")
			.append("void " + className + "::ensureLoaded() {\n")
			.append("\tif(!sLoaded) {\n")
			.append("\t\tstring fullPath = CCUtils::getExternalOrFullPath(\"" + className + ".json\");\n")
			.append("\t\tchar* raw = CCResourceLoader::loadCString(fullPath, decryptRes);\n")
			.append("\t\tJson::Reader reader;\n")
			.append("\t\treader.parse(raw, sJSON);\n")
			.append("\t\tfree(raw);\n")
			.append("\t\tsLoaded = true;\n")
			.append("\t}\n")
			.append("}\n")
			.append("\n")
			.append(className + "* " + className + "::create(" + (idIsString ? "const string&" : "int") + " key) {\n")
			.append("\tensureLoaded();\n")
			.append("\t" + className + "* instance = new " + className + "();\n")
			.append("\tif(instance->initWithKey(key)) {\n")
			.append("\t\treturn (" + className + "*)instance->autorelease();\n")
			.append("\t}\n")
			.append("\tCC_SAFE_RELEASE(instance);\n")
			.append("\treturn NULL;\n")
			.append("}\n")
			.append("\n")
			.append(className + "* " + className + "::createWithIndex(int index) {\n")
			.append("\tensureLoaded();\n")
			.append("\tif(index < 0 || index >= count()) {\n")
			.append("\t\treturn NULL;\n")
			.append("\t} else {\n")
			.append("\t\t" + className + "* instance = new " + className + "();\n")
			.append("\t\tif(instance->initWithIndex(index)) {\n")
			.append("\t\t\treturn (" + className + "*)instance->autorelease();\n")
			.append("\t\t}\n")
			.append("\t\tCC_SAFE_RELEASE(instance);\n")
			.append("\t\treturn NULL;\n")
			.append("\t}\n")
			.append("}\n")
			.append("\n")
			.append(className + "* " + className + "::createEmpty(" + (idIsString ? "const string&" : "int") + " _id) {\n")
			.append("\t" + className + "* instance = new " + className + "();\n")
			.append("\tif(instance->initWithId(_id)) {\n")
			.append("\t\treturn (" + className + "*)instance->autorelease();\n")
			.append("\t}\n")
			.append("\tCC_SAFE_RELEASE(instance);\n")
			.append("\treturn NULL;\n")
			.append("}\n")
			.append("\n")
			.append("int " + className + "::count() {\n")
			.append("\tensureLoaded();\n")
			.append("\treturn sJSON.size();\n")
			.append("}\n")
			.append("\n")
			.append("Json::Value& " + className + "::query(const string& path) {\n")
			.append("\tensureLoaded();\n")
			.append("\tJson::Path p(path);\n")
			.append("\treturn p.make(sJSON);\n")
			.append("}\n")
			.append("\n")
			.append("int " + className + "::indexOf(" + (idIsString ? "const string&" : "int") + " _id) {\n")
			.append("\tint size = count();\n")
			.append("\tint index = 0;\n")
			.append("\tJson::ValueIterator iter = sJSON.begin();\n")
			.append("\twhile(index < size) {\n")
			.append("\t\tconst Json::Value& item = *iter;\n")
			.append("\t\tif(item[\"" + firstCapital(idName) + "\"]." + (idIsString ? "asString" : "asInt") + "() == _id) {\n")
			.append("\t\t\treturn index;\n")
			.append("\t\t} else {\n")
			.append("\t\t\titer++;\n")
			.append("\t\t\tindex++;\n")
			.append("\t\t}\n")
			.append("\t}\n")
			.append("\treturn -1;\n")
			.append("}\n")
			.append("\n")
			.append("int " + className + "::indexOf(" + className + "* t) {\n")
			.append("\tint size = count();\n")
			.append("\tint index = 0;\n")
			.append("\tJson::ValueIterator iter = sJSON.begin();\n")
			.append("\twhile(index < size) {\n")
			.append("\t\tconst Json::Value& item = *iter;\n")
			.append("\t\tif(item[\"" + firstCapital(idName) + "\"]." + (idIsString ? "asString" : "asInt") + "() == t->get" + firstCapital(idName) + "()) {\n")
			.append("\t\t\treturn index;\n")
			.append("\t\t} else {\n")
			.append("\t\t\titer++;\n")
			.append("\t\t\tindex++;\n")
			.append("\t\t}\n")
			.append("\t}\n")
			.append("\treturn -1;\n")
			.append("}\n")
			.append("\n")
			.append("bool " + className + "::initWithIndex(int index) {\n")
			.append("\tJson::ValueIterator iter = sJSON.begin();\n")
			.append("\twhile(index-- > 0)\n")
			.append("\t\titer++;\n")
			.append("\treturn initWithValue(*iter);\n")
			.append("}\n")
			.append("\n")
			.append("bool " + className + "::initWithKey(" + (idIsString ? "const string&" : "int") + " key) {\n")
			.append(idIsString ? "\tconst char* _key = key.c_str();\n" : "\tchar _key[64];\n")
			.append(idIsString ? "" : "\tsprintf(_key, \"%d\", key);\n")
			.append("\tif(sJSON.isMember(_key)) {\n")
			.append("\t\treturn initWithValue(sJSON[_key]);\n")
			.append("\t} else {\n")
			.append("\t\treturn false;\n")
			.append("\t}\n")
			.append("}\n")
			.append("\n")
			.append("bool " + className + "::initWithValue(const Json::Value& item) {\n");

		// body of initWithValue
		for (int i = 0; i < len; i++) {
			if (sheet.getRow(3).getCell(i) == null || sheet.getRow(3).getCell(i).getStringCellValue().equals(""))
				continue;
			String field = sheet.getRow(2).getCell(i).getStringCellValue();
			String dataType = sheet.getRow(3).getCell(i).getStringCellValue();
			if (field == null || field.equals(""))
				continue;
			if (dataType.equalsIgnoreCase("Byte") || dataType.equalsIgnoreCase("int")) {
				cfile.append("\tm_" + firstLowercase(field) + " = item[\"" + firstCapital(field) + "\"].asInt();\n");
			} else if (dataType.equalsIgnoreCase("bool")) {
				cfile.append("\tm_" + firstLowercase(field) + " = item[\"" + firstCapital(field) + "\"].asBool();\n");
			} else if (dataType.equalsIgnoreCase("Float")) {
				cfile.append("\tm_" + firstLowercase(field) + " = item[\"" + firstCapital(field) + "\"].asDouble();\n");
			} else if (dataType.equalsIgnoreCase("String")) {
				cfile.append("\tm_" + firstLowercase(field) + " = item[\"" + firstCapital(field) + "\"].asString();\n");
			} else if(dataType.equalsIgnoreCase("luafunc")) {
				cfile.append("\tm_" + firstLowercase(field) + " = item[\"" + firstCapital(field) + "\"].asString();\n");
				cfile.append("\tm_" + firstLowercase(field) + " = CCUtils::replace(m_" + firstLowercase(field) + ", \"\\\\n\", \"\\n\");\n");
				cfile.append("\tm_" + firstLowercase(field) + " = CCUtils::replace(m_" + firstLowercase(field) + ", \"\\\\r\", \"\\r\");\n");
			} else if(dataType.equalsIgnoreCase("StringArray")) {
				cfile.append("\tCCArray& tmp_" + firstLowercase(field) + " = CCUtils::componentsOfString(item[\"" + firstCapital(field) + "\"].asString(), ',');\n");
				cfile.append("\tm_" + firstLowercase(field) + ".addObjectsFromArray(&tmp_" + firstLowercase(field) + ");\n");
			} else if(dataType.equalsIgnoreCase("IntArray")) {
				cfile.append("\tCCArray& tmp_" + firstLowercase(field) + " = CCUtils::intComponentsOfString(item[\"" + firstCapital(field) + "\"].asString(), ',');\n");
				cfile.append("\tm_" + firstLowercase(field) + ".addObjectsFromArray(&tmp_" + firstLowercase(field) + ");\n");
			} else if(dataType.equalsIgnoreCase("FloatArray")) {
				cfile.append("\tCCArray& tmp_" + firstLowercase(field) + " = CCUtils::floatComponentsOfString(item[\"" + firstCapital(field) + "\"].asString(), ',');\n");
				cfile.append("\tm_" + firstLowercase(field) + ".addObjectsFromArray(&tmp_" + firstLowercase(field) + ");\n");
			} else if(dataType.equalsIgnoreCase("BoolArray")) {
				cfile.append("\tCCArray& tmp_" + firstLowercase(field) + " = CCUtils::boolComponentsOfString(item[\"" + firstCapital(field) + "\"].asString(), ',');\n");
				cfile.append("\tm_" + firstLowercase(field) + ".addObjectsFromArray(&tmp_" + firstLowercase(field) + ");\n");
			}
		}
		
		// close initWithValue and start initWithId
		cfile.append("\treturn true;\n")
			.append("}\n")
			.append("\n")
			.append("bool " + className + "::initWithId(" + (idIsString ? "const string&" : "int") + " _id) {\n")
			.append("\tm_" + firstLowercase(idName) + " = _id;\n");
		
		// body of initWithId
		for (int i = 0; i < len; i++) {
			if (sheet.getRow(3).getCell(i) == null || sheet.getRow(3).getCell(i).getStringCellValue().equals(""))
				continue;
			String field = sheet.getRow(2).getCell(i).getStringCellValue();
			String dataType = sheet.getRow(3).getCell(i).getStringCellValue();
			if (field == null || field.equals("") || idName.equals(field))
				continue;
			if (dataType.equalsIgnoreCase("Byte") || dataType.equalsIgnoreCase("int") || dataType.equalsIgnoreCase("Float")) {
				cfile.append("\tm_" + firstLowercase(field) + " = 0;\n");
			} else if (dataType.equalsIgnoreCase("bool")) {
				cfile.append("\tm_" + firstLowercase(field) + " = false;\n");
			}
		}
		
		// close initWithId and start toValue
		cfile.append("\treturn true;\n")
			.append("}\n")
			.append("\n")
			.append("Json::Value " + className + "::toValue() {\n")
			.append("\tJson::Value v;\n");
			
		// body of toValue
		for (int i = 0; i < len; i++) {
			if (sheet.getRow(3).getCell(i) == null || sheet.getRow(3).getCell(i).getStringCellValue().equals(""))
				continue;
			String field = sheet.getRow(2).getCell(i).getStringCellValue();
			String dataType = sheet.getRow(3).getCell(i).getStringCellValue();
			if (field == null || field.equals(""))
				continue;
			if(dataType.equalsIgnoreCase("luafunc")) {
				cfile.append("\tstring tmp_" + firstLowercase(field) + " = CCUtils::replace(m_" + firstLowercase(field) + ", \"\\r\", \"\\\\r\");\n");
				cfile.append("\ttmp_" + firstLowercase(field) + " = CCUtils::replace(tmp_" + firstLowercase(field) + ", \"\\n\", \"\\\\n\");\n");
				cfile.append("\tv[\"" + firstCapital(field) + "\"] = tmp_" + firstLowercase(field) + ";\n");	
			} else if(dataType.equalsIgnoreCase("StringArray")) {
				cfile.append("\tv[\"" + firstCapital(field) + "\"] = CCUtils::joinString(m_" + firstLowercase(field) + ", ',');\n");	
			} else if(dataType.equalsIgnoreCase("IntArray")) {
				cfile.append("\tv[\"" + firstCapital(field) + "\"] = CCUtils::joinInt(m_" + firstLowercase(field) + ", ',');\n");	
			} else if(dataType.equalsIgnoreCase("FloatArray")) {
				cfile.append("\tv[\"" + firstCapital(field) + "\"] = CCUtils::joinFloat(m_" + firstLowercase(field) + ", ',');\n");	
			} else if(dataType.equalsIgnoreCase("BoolArray")) {
				cfile.append("\tv[\"" + firstCapital(field) + "\"] = CCUtils::joinBool(m_" + firstLowercase(field) + ", ',');\n");	
			} else {
				cfile.append("\tv[\"" + firstCapital(field) + "\"] = m_" + firstLowercase(field) + ";\n");	
			}
		}
		
		// close toValue
		cfile.append("\treturn v;\n")
			.append("}\n");
		
		// get method for array types
		for (int i = 0; i < len; i++) {
			if (sheet.getRow(3).getCell(i) == null || sheet.getRow(3).getCell(i).getStringCellValue().equals(""))
				continue;
			String field = sheet.getRow(2).getCell(i).getStringCellValue();
			String dataType = sheet.getRow(3).getCell(i).getStringCellValue();
			if (field == null || field.equals(""))
				continue;
			if(dataType.equalsIgnoreCase("StringArray")) {
				cfile.append("\nstring " + className + "::get" + firstCapital(field) + "At(int index) {\n")
					.append("\tif(index < 0 || index >= m_" + firstLowercase(field) + ".count())\n")
					.append("\t\treturn \"\";\n")
					.append("\tCCString* cc = (CCString*)m_" + firstLowercase(field) + ".objectAtIndex(index);\n")
					.append("\treturn cc->getCString();\n")
					.append("}\n");
			} else if(dataType.equalsIgnoreCase("IntArray")) {
				cfile.append("\nint " + className + "::get" + firstCapital(field) + "At(int index) {\n")
					.append("\tif(index < 0 || index >= m_" + firstLowercase(field) + ".count())\n")
					.append("\t\treturn 0;\n")
					.append("\tCCInteger* cc = (CCInteger*)m_" + firstLowercase(field) + ".objectAtIndex(index);\n")
					.append("\treturn cc->getValue();\n")
					.append("}\n");
			} else if(dataType.equalsIgnoreCase("FloatArray")) {
				cfile.append("\nfloat " + className + "::get" + firstCapital(field) + "At(int index) {\n")
					.append("\tif(index < 0 || index >= m_" + firstLowercase(field) + ".count())\n")
					.append("\t\treturn 0;\n")
					.append("\tCCFloat* cc = (CCFloat*)m_" + firstLowercase(field) + ".objectAtIndex(index);\n")
					.append("\treturn cc->getValue();\n")
					.append("}\n");
			} else if(dataType.equalsIgnoreCase("BoolArray")) {
				cfile.append("\nbool " + className + "::get" + firstCapital(field) + "At(int index) {\n")
					.append("\tif(index < 0 || index >= m_" + firstLowercase(field) + ".count())\n")
					.append("\t\treturn false;\n")
					.append("\tCCBool* cc = (CCBool*)m_" + firstLowercase(field) + ".objectAtIndex(index);\n")
					.append("\treturn cc->getValue();\n")
					.append("}\n");
			}
		}

		try {
			File dstFile = new File(outputDir, className + ".cpp");
			writeFile(dstFile.getAbsolutePath(), cfile.toString());
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public boolean getFileList(Vector<String> outFileLists, String filePath, boolean subFolderFlag) {
		if (outFileLists == null) {
			outFileLists = new Vector<String>();
		}
		File file = new File(filePath);
		if (file.exists()) {
			File files[] = file.listFiles();
			if (subFolderFlag) {
				for (int i = 0; i < files.length; i++) {
					if (files[i].isFile()) {
						String fileName = files[i].getName();
						if (fileName.indexOf(".xls") > -1 || fileName.indexOf("xlsx") > -1) {
							outFileLists.add(fileName);
						}
					} else if (files[i].isDirectory()) {
						getFileList(outFileLists, filePath + "//" + files[i].getName(), subFolderFlag);
					}
				}
			} else {
				for (int i = 0; i < files.length; i++) {
					String fileName = files[i].getName();
					if (files[i].isFile() && (fileName.indexOf(".xls") > -1 || fileName.indexOf("xlsx") > -1)) {
						outFileLists.add(fileName);
					}
				}
			}
		} else {
			return false;
		}
		return true;
	}
}
