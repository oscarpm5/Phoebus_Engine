#ifndef __CONFIG_H__
#define __CONFIG_H__


#include <string>
#include "Globals.h"
#include <vector>
#include "MathGeoLib/include/MathGeoLib.h"
//#include "Json/parson.h"

//http://kgabis.github.io/parson/


//TODO FOR ADRI: For the love of God almighty, clean this shit. We have like 10x the amount of funcs we need

struct json_object_t;
//typedef struct json_object_t JSON_Object;

struct json_value_t;
//typedef struct json_value_t  JSON_Value;

struct json_array_t;
//typedef struct json_array_t  JSON_Array;

class Config_Array;
class Config
{
	///Brief parson explanation
	/*
	-	Save a JSON file into a buffer then create a new file with fileSystem:
		JSON_Status json_serialize_to_buffer(const JSON_Value *value, char *buf, size_t buf_size_in_bytes);
	-	JSON nodes are called objects
	-	Append attributes to object: json_object_set_[type of attribute]. Attribute types are bool, number, string and value (any attribute or an object)
	-	Get values from a node: same function as append, but with "get"
	*/

public:
	Config();						//Contructor used for new files
	Config(const char* buffer);		//Constructor used for data read
	Config(json_object_t* obj);		//Constructor used for node append
	~Config();						//Free data if initialized

	uint Serialize(char** buffer);	//Returns a filled buffer
	bool NodeExists();
	void Release();

	//Append attributes -----------
	void SetNumber(const char* name, double data);
	void SetString(const char* name, const char* data);
	void SetBool(const char* name, bool data);
	Config_Array SetArray(const char* name);
	Config SetNode(const char* name);
	//Endof append attributes------

	//Get attributes --------------
	double GetNumber(const char* name, double default = 0) const;
	std::string GetString(const char* name, const char* default = "") const;
	bool GetBool(const char* name, bool default = true) const;
	Config_Array GetArray(const char* name) const;
	Config GetNode(const char* name) const;
	//Endof Get attributes---------

private:
	json_value_t* root_value = nullptr; //Only used for file root
	json_object_t* node = nullptr;
};

class Config_Array
{
public:
	//Contructor only to be called from Config, it would cause mem leak
	Config_Array();
	Config_Array(json_array_t* arr);

	//Append attributes ------------
	void AddNumber(double number);
	void AddString(char* string);
	void AddBool(bool boolean);
	void AddFloat3(const float3& data);
	void AddFloat4(const float4& data);
	void Add4x4Mat(const float4x4& trans);

	void AddQuat(const Quat& data);
	Config AddNode();
	//Endof append attributes-------

	//Get attributes ---------------
	double GetNumber(uint index, double default = 0) const;
	const char* GetString(uint index, const char* default = "") const;
	bool GetBool(uint index, bool default = true) const;
	float3 GetFloat3(uint index, float3 default = float3::zero) const; //Index is based on float3 not on single data!
	float4 GetFloat4(uint index, float4 default = float4::zero) const; //Index is based on float4 not on single data!
	Quat GetQuat(uint index, Quat  default = Quat::identity) const;
	float4x4 GetMatTransform(uint index, float4x4 default = float4x4::identity) const;
	void FillVectorNumber(std::vector<double>& vector) const;
	void FillVectorString(std::vector<char*>& vector) const;
	void FillVectorBoool(std::vector<bool>& vector) const;
	Config GetNode(uint index) const;
	uint GetSize() const;
	//Endof Get attributes----------

private:
	json_array_t* arr;
	uint size = 0;
};

#endif //__CONFIG_H__
#pragma once
