#include "GAMESYSTEM.h"
#include "MODEL.h"
#include <fstream>
#include <stdlib.h>
#pragma warning (disable:4267)
string MODEL::defaltPath = "Data\\Model\\obj\\";

void MODEL::Initialize(std::string _filename, D3DXMATRIXA16 _position)
{
	gSystem.console.SetFunction("MODEL::Initialize");
	bEnable = true;
	position = _position;
	int pos = _filename.find(".obj");
	if (pos >= 0 && pos < _filename.length())
		fileType = OBJ;
	pos = _filename.find(".orm");
	if (pos >= 0 && pos < _filename.length())
		fileType = ORM;
	if (fileType == -1)
		throw RUNTIME_ERROR(MODEL_LOAD_ERROR);

	model_name = _filename;
	model_name.pop_back();
	model_name.pop_back();
	model_name.pop_back();
	model_name.pop_back();
	path = defaltPath + model_name + "\\";

	if (fileType == OBJ)
		ReadObj();
	else if (fileType == ORM)
		ReadOrm();
	else
	{
		gSystem.console << con::error << con::func << "file type error : " << _filename << con::endl;
		bEnable = false;
		throw RUNTIME_ERROR(MODEL_LOAD_ERROR);
	}
	gSystem.console.RestoreFunction();
}
int MODEL::cStrFind(char* _ori, const char* _search)
{
	for (int i = 0; _ori[i] != '\0'; i++)
	{
		if (_ori[i] == _search[0])
		{
			bool check = true;
			for (int j = 1; _search[j] != '\0'; j++)
			{
				if (_ori[i + j] != _search[j])
				{
					check = false;
					break;
				}
			}
			if (check)
				return i;
		}
	}
	return -1;
}

void MODEL::ReadObj()
{
	gSystem.console.SetFunction("MODEL::ReadObj");
	FILE* objfile;
	vector<MATRIX3> temp_vertex, temp_normal;
	vector<MATRIX2> temp_texture;
	temp_vertex.push_back(MATRIX3(0, 0, 0));
	temp_normal.push_back(MATRIX3(0, 0, 0));
	temp_texture.push_back(MATRIX2(0, 0));

	char* strtof_end_ptr;
	char read_line[128];
	bool mtl_find_check = false;

	objfile = fopen((path + model_name + ".obj").c_str(), "r");
	if (objfile == NULL)
	{
		gSystem.console << con::error << con::func << "cannot find file : " << path + model_name + ".obj" << con::endl;
		gSystem.console << con::error << con::func << "release model : " << model_name + ".obj" << con::endl;
		bEnable = false;
		throw RUNTIME_ERROR(MODEL_LOAD_ERROR);
	}

	while (!feof(objfile))
	{
		fgets(read_line, sizeof(read_line), objfile);
		//	cout << readLine << endl;
		if (cStrFind(read_line, "mtllib ") == 0)
		{
			if (read_line[7] != 10)
			{
				ReadMtl(path + (read_line + 7));
				mtl_find_check = true;
			}
		}

		else if (cStrFind(read_line, "usemtl ") == 0)
		{
			string mtl_name(read_line + 7);
			MESH temp_mesh;
			temp_mesh.VB = NULL;
			temp_mesh.faceCount = 0;
			for (temp_mesh.mtl = mtl.begin(); temp_mesh.mtl != mtl.end(); temp_mesh.mtl++)
			{
				if (*temp_mesh.mtl->name == mtl_name)
					break;
			}
			if (temp_mesh.mtl == mtl.end())
			{
				gSystem.console << con::error << con::func << "cannot find material : " << mtl_name << con::endl;
				gSystem.console << con::error << con::func << "error line : " << read_line << con::endl;
				gSystem.console << con::error << con::func << "release model : " << model_name + ".obj" << con::endl;
				bEnable = false;
				throw RUNTIME_ERROR(MODEL_LOAD_ERROR);
			}

			char temp_next_line[128];
			fgets(temp_next_line, sizeof(temp_next_line), objfile);
			while (cStrFind(temp_next_line, "f ") != 0)
			{
				fgets(temp_next_line, sizeof(temp_next_line), objfile);
			}
			int first_f_position = -1, second_f_position = -1;

			for (int i = 2; temp_next_line[i] != '\0' && temp_next_line[i] != '\n'; i++)
			{
				if (temp_next_line[i] == '/')
				{
					if (first_f_position == -1)
						first_f_position = i;
					else
					{
						second_f_position = i;
						break;
					}
				}
			}
			if (first_f_position == -1 && second_f_position == -1)
				temp_mesh.FVF = VertexXYZ::FVF;
			else if (first_f_position != -1 && second_f_position == -1)
				temp_mesh.FVF = VertexXYZTEX::FVF;
			else if (first_f_position != -1 && second_f_position == first_f_position + 1)
				temp_mesh.FVF = VertexXYZNOR::FVF;
			else if (first_f_position != -1 && second_f_position != -1)
				temp_mesh.FVF = VertexXYZTEXNOR::FVF;

			mesh.push_back(temp_mesh);
			ReadFace(temp_next_line);
		}
		else if (cStrFind(read_line, "f ") == 0)
		{
			if (mtl_find_check == false)
			{
				MTL temp_mtl;
				temp_mtl.name = new string("default MTL");
				temp_mtl.texture = NULL;
				temp_mtl.illum = false;
				mtl.push_back(temp_mtl);
				mtl.back().material.ambient.x = 1.0f;
				mtl.back().material.ambient.y = 1.0f;
				mtl.back().material.ambient.z = 1.0f;
				mtl.back().material.ambient.w = 1.0f;
				mtl.back().material.diffuse.x = 1.0f;
				mtl.back().material.diffuse.y = 1.0f;
				mtl.back().material.diffuse.z = 1.0f;
				mtl.back().material.diffuse.w = 1.0f;
				mtl.back().material.specular.x = 1.0f;
				mtl.back().material.specular.y = 1.0f;
				mtl.back().material.specular.z = 1.0f;
				mtl.back().material.specular.w = 1.0f;
				gSystem.texture[*mtl.back().name] = NULL;
				mtl.back().texture = gSystem.default_texture;

				MESH temp_mesh;
				temp_mesh.VB = NULL;
				temp_mesh.faceCount = 0;
				temp_mesh.mtl = --mtl.end();

				char temp_next_line[128];
				fgets(temp_next_line, sizeof(temp_next_line), objfile);
				while (cStrFind(temp_next_line, "f ") != 0)
				{
					fgets(temp_next_line, sizeof(temp_next_line), objfile);
				}
				int first_f_position = -1, second_f_position = -1;

				for (int i = 2; temp_next_line[i] != '\0' && temp_next_line[i] != '\n'; i++)
				{
					if (temp_next_line[i] == '/')
					{
						if (first_f_position == -1)
							first_f_position = i;
						else
						{
							second_f_position = i;
							break;
						}
					}
				}
				if (first_f_position == -1 && second_f_position == -1)
					temp_mesh.FVF = VertexXYZ::FVF;
				else if (first_f_position != -1 && second_f_position == -1)
					temp_mesh.FVF = VertexXYZTEX::FVF;
				else if (first_f_position != -1 && second_f_position == first_f_position + 1)
					temp_mesh.FVF = VertexXYZNOR::FVF;
				else if (first_f_position != -1 && second_f_position != -1)
					temp_mesh.FVF = VertexXYZTEXNOR::FVF;

				mesh.push_back(temp_mesh);
				mtl_find_check = true;
				ReadFace(temp_next_line);
			}
			ReadFace(read_line);
			read_line[0] = '\0';
		}
		else if (cStrFind(read_line, "v ") == 0)
		{
			float tmp1, tmp2, tmp3;
			tmp1 = strtof(read_line + 2, &strtof_end_ptr);
			if (*strtof_end_ptr == '\n')
			{
				gSystem.console << con::error << con::func << "obj file error : " << model_name + ".obj" << con::endl;
				gSystem.console << con::error << con::func << "error line : " << read_line << con::endl;
				gSystem.console << con::error << con::func << "release model : " << model_name + ".obj" << con::endl;
				temp_vertex.~vector(); temp_normal.~vector(); temp_texture.~vector();
				bEnable = false;
				throw RUNTIME_ERROR(MODEL_LOAD_ERROR);
			}
			tmp2 = strtof(strtof_end_ptr, &strtof_end_ptr);
			if (*strtof_end_ptr == '\n')
			{
				gSystem.console << con::error << con::func << "obj file error : " << model_name + ".obj" << con::endl;
				gSystem.console << con::error << con::func << "error line : " << read_line << con::endl;
				gSystem.console << con::error << con::func << "release model : " << model_name + ".obj" << con::endl;
				temp_vertex.~vector(); temp_normal.~vector(); temp_texture.~vector();
				bEnable = false;
				throw RUNTIME_ERROR(MODEL_LOAD_ERROR);
			}
			tmp3 = strtof(strtof_end_ptr, NULL);
			//	cout << tmp1 << " " << tmp2 << " " << tmp3 << endl;
			temp_vertex.push_back(MATRIX3(tmp1, tmp2, tmp3*-1.0f));
		}
		else if (cStrFind(read_line, "vt ") == 0)
		{
			float tmp1, tmp2;
			tmp1 = strtof(read_line + 2, &strtof_end_ptr);
			if (*strtof_end_ptr == '\n')
			{
				gSystem.console << con::error << con::func << "obj file error : " << model_name + ".obj" << con::endl;
				gSystem.console << con::error << con::func << "error line : " << read_line << con::endl;
				gSystem.console << con::error << con::func << "release model : " << model_name + ".obj" << con::endl;
				temp_vertex.~vector(); temp_normal.~vector(); temp_texture.~vector();
				bEnable = false;
				throw RUNTIME_ERROR(MODEL_LOAD_ERROR);
			}
			tmp2 = strtof(strtof_end_ptr, NULL);
			temp_texture.push_back(MATRIX2(tmp1, 1.0f - tmp2));
		}
		else if (cStrFind(read_line, "vn ") == 0)
		{
			float tmp1, tmp2, tmp3;
			tmp1 = strtof(read_line + 2, &strtof_end_ptr);
			if (*strtof_end_ptr == '\n')
			{
				gSystem.console << con::error << con::func << "obj file error : " << model_name + ".obj" << con::endl;
				gSystem.console << con::error << con::func << "error line : " << read_line << con::endl;
				gSystem.console << con::error << con::func << "release model : " << model_name + ".obj" << con::endl;
				temp_vertex.~vector(); temp_normal.~vector(); temp_texture.~vector();
				bEnable = false;
				throw RUNTIME_ERROR(MODEL_LOAD_ERROR);
			}
			tmp2 = strtof(strtof_end_ptr, &strtof_end_ptr);
			if (*strtof_end_ptr == '\n')
			{
				gSystem.console << con::error << con::func << "obj file error : " << model_name + ".obj" << con::endl;
				gSystem.console << con::error << con::func << "error line : " << read_line << con::endl;
				gSystem.console << con::error << con::func << "release model : " << model_name + ".obj" << con::endl;
				temp_vertex.~vector(); temp_normal.~vector(); temp_texture.~vector();
				bEnable = false;
				throw RUNTIME_ERROR(MODEL_LOAD_ERROR);
			}
			tmp3 = strtof(strtof_end_ptr, NULL);
			temp_normal.push_back(MATRIX3(tmp1, tmp2, tmp3*-1.0f));
		}
	}

	fclose(objfile);

	objfile = fopen((path + model_name + ".obj").c_str(), "r");
	while (!feof(objfile))
	{
		fgets(read_line, sizeof(read_line), objfile);

		if (cStrFind(read_line, "usemtl ") == 0)
			break;
	}
	for (auto meshPtr = mesh.begin(); meshPtr != mesh.end(); meshPtr++)
	{
		if (meshPtr->FVF == VertexXYZTEXNOR::FVF)
		{
			VertexXYZTEXNOR* data_;
			gSystem.device->CreateVertexBuffer(meshPtr->faceCount * 3 * sizeof(VertexXYZTEXNOR), D3DUSAGE_WRITEONLY, VertexXYZTEXNOR::FVF, D3DPOOL_MANAGED, &meshPtr->VB, 0);
			if (meshPtr->VB == NULL)
			{
				gSystem.console << con::error << con::func << "CreateVertexBuffer() - failed" << con::endl;
				gSystem.console << con::error << con::func << "release model : " << model_name + ".obj" << con::endl;
				temp_vertex.~vector(); temp_normal.~vector(); temp_texture.~vector();
				bEnable = false;
				throw RUNTIME_ERROR(MODEL_LOAD_ERROR);
			}
			meshPtr->VB->Lock(0, 0, (void**)&data_, 0);
			int data_count = 0;
			while (!feof(objfile))
			{
				fgets(read_line, sizeof(read_line), objfile);
				if (cStrFind(read_line, "f ") == 0)
				{
					int tmp[9];
					int before = 2;
					int count = 0;
					for (int i = 2;; i++)
					{
						if (read_line[i] < 48 || read_line[i] > 57)
						{
							if (before != i)
							{
								int out = 0;
								for (int j = before; j != i; j++)
								{
									out = out * 10 + read_line[j] - 48;
								}
								before = i + 1;
								tmp[count++] = out;
								if (count == 9)
								{
									//	cout << "f data " << endl;
									//	cout << " : " << tmp[0] << " " << tmp[1] << " " << tmp[2] << endl;
									//	cout << tmpV[tmp[0]].x << " " << tmpV[tmp[0]].y << " " << tmpV[tmp[0]].z << endl;
									//	cout << tmpT[tmp[1]].u << " " << tmpT[tmp[1]].v << endl;
									//	cout << tmpN[tmp[2]].x << " " << tmpN[tmp[2]].y << " " << tmpN[tmp[2]].z << endl;

									//	cout << " : " << tmp[3] << " " << tmp[4] << " " << tmp[5] << endl;
									//	cout << tmpV[tmp[3]].x << " " << tmpV[tmp[3]].y << " " << tmpV[tmp[3]].z << endl;
									//	cout << tmpT[tmp[4]].u << " " << tmpT[tmp[4]].v << endl;
									//	cout << tmpN[tmp[5]].x << " " << tmpN[tmp[5]].y << " " << tmpN[tmp[5]].z << endl;

									//	cout << " : " << tmp[6] << " " << tmp[7] << " " << tmp[8] << endl;
									//	cout << tmpV[tmp[6]].x << " " << tmpV[tmp[6]].y << " " << tmpV[tmp[6]].z << endl;
									//	cout << tmpT[tmp[7]].u << " " << tmpT[tmp[7]].v << endl;
									//	cout << tmpN[tmp[8]].x << " " << tmpN[tmp[8]].y << " " << tmpN[tmp[8]].z << endl;
									data_[data_count++] = VertexXYZTEXNOR(
										temp_vertex[tmp[0]].x, temp_vertex[tmp[0]].y, temp_vertex[tmp[0]].z,
										temp_texture[tmp[1]].u, temp_texture[tmp[1]].v,
										temp_normal[tmp[2]].x, temp_normal[tmp[2]].y, temp_normal[tmp[2]].z);
									data_[data_count++] = VertexXYZTEXNOR(
										temp_vertex[tmp[6]].x, temp_vertex[tmp[6]].y, temp_vertex[tmp[6]].z,
										temp_texture[tmp[7]].u, temp_texture[tmp[7]].v,
										temp_normal[tmp[8]].x, temp_normal[tmp[8]].y, temp_normal[tmp[8]].z);
									data_[data_count++] = VertexXYZTEXNOR(
										temp_vertex[tmp[3]].x, temp_vertex[tmp[3]].y, temp_vertex[tmp[3]].z,
										temp_texture[tmp[4]].u, temp_texture[tmp[4]].v,
										temp_normal[tmp[5]].x, temp_normal[tmp[5]].y, temp_normal[tmp[5]].z);

									count = 6;
									tmp[3] = tmp[6]; tmp[4] = tmp[7]; tmp[5] = tmp[8];
								}
							}
							if (read_line[i] == '\n' || feof(objfile))
								break;
						}
					}
					read_line[0] = '\0';
				}
				else if (cStrFind(read_line, "usemtl ") == 0)
				{
					meshPtr->VB->Unlock();
					break;
				}
			}
			if (meshPtr == --mesh.end())
			{
				meshPtr->VB->Unlock();
				break;
			}
		}
		else if (meshPtr->FVF == VertexXYZNOR::FVF)
		{
			VertexXYZNOR* data_;
			gSystem.device->CreateVertexBuffer(meshPtr->faceCount * 3 * sizeof(VertexXYZNOR), D3DUSAGE_WRITEONLY, meshPtr->FVF, D3DPOOL_MANAGED, &meshPtr->VB, 0);
			if (meshPtr->VB == NULL)
			{
				gSystem.console << con::error << con::func << "CreateVertexBuffer() - failed" << con::endl;
				gSystem.console << con::error << con::func << "release model : " << model_name + ".obj" << con::endl;
				temp_vertex.~vector(); temp_normal.~vector(); temp_texture.~vector();
				bEnable = false;
				throw RUNTIME_ERROR(MODEL_LOAD_ERROR);
			}
			meshPtr->VB->Lock(0, 0, (void**)&data_, 0);
			int data_count = 0;
			while (!feof(objfile))
			{
				fgets(read_line, sizeof(read_line), objfile);
				if (cStrFind(read_line, "f ") == 0)
				{
					int tmp[6];
					int before = 2;
					int count = 0;
					for (int i = 2;; i++)
					{
						if (read_line[i] < 48 || read_line[i] > 57)
						{
							if (before != i)
							{
								int out = 0;
								for (int j = before; j != i; j++)
								{
									out = out * 10 + read_line[j] - 48;
								}
								if (read_line[i + 1] < 48 || read_line[i + 1] > 57)
									i++;
								before = i + 1;
								tmp[count++] = out;
								if (count == 6)
								{
									//		cout << "f data " << endl;
									//		cout << " : " << tmp[0] << " " << tmp[1] << endl;
									//		cout << tmpV[tmp[0]].x << " " << tmpV[tmp[0]].y << " " << tmpV[tmp[0]].z << endl;
									//		cout << tmpN[tmp[1]].x << " " << tmpN[tmp[1]].y << " " << tmpN[tmp[1]].z << endl;
									//
									//		cout << " : " << tmp[2] << " " << tmp[3] << endl;
									//		cout << tmpV[tmp[2]].x << " " << tmpV[tmp[2]].y << " " << tmpV[tmp[2]].z << endl;
									//		cout << tmpN[tmp[3]].x << " " << tmpN[tmp[3]].y << " " << tmpN[tmp[3]].z << endl;
									//
									//		cout << " : " << tmp[4] << " " << tmp[5] << endl;
									//		cout << tmpV[tmp[4]].x << " " << tmpV[tmp[4]].y << " " << tmpV[tmp[4]].z << endl;
									//		cout << tmpN[tmp[5]].x << " " << tmpN[tmp[5]].y << " " << tmpN[tmp[5]].z << endl << endl;
									data_[data_count++] = VertexXYZNOR(
										temp_vertex[tmp[0]].x, temp_vertex[tmp[0]].y, temp_vertex[tmp[0]].z,
										temp_normal[tmp[1]].x, temp_normal[tmp[1]].y, temp_normal[tmp[1]].z);
									data_[data_count++] = VertexXYZNOR(
										temp_vertex[tmp[4]].x, temp_vertex[tmp[4]].y, temp_vertex[tmp[4]].z,
										temp_normal[tmp[5]].x, temp_normal[tmp[5]].y, temp_normal[tmp[5]].z);
									data_[data_count++] = VertexXYZNOR(
										temp_vertex[tmp[2]].x, temp_vertex[tmp[2]].y, temp_vertex[tmp[2]].z,
										temp_normal[tmp[3]].x, temp_normal[tmp[3]].y, temp_normal[tmp[3]].z);

									count = 4;
									tmp[2] = tmp[4]; tmp[3] = tmp[5];
								}
							}
							if (read_line[i - 1] == 10 || feof(objfile))
								break;
						}
					}
					read_line[0] = '\0';
				}
				else if (cStrFind(read_line, "usemtl ") == 0)
				{
					meshPtr->VB->Unlock();
					break;
				}
			}
			if (meshPtr == --mesh.end())
			{
				meshPtr->VB->Unlock();
				break;
			}
		}
		else if (meshPtr->FVF == VertexXYZTEX::FVF)
		{
			VertexXYZTEX* data_;
			gSystem.device->CreateVertexBuffer(meshPtr->faceCount * 3 * sizeof(VertexXYZTEX), D3DUSAGE_WRITEONLY, meshPtr->FVF, D3DPOOL_MANAGED, &meshPtr->VB, 0);
			if (meshPtr->VB == NULL)
			{
				gSystem.console << con::error << con::func << "CreateVertexBuffer() - failed" << con::endl;
				gSystem.console << con::error << con::func << "release model : " << model_name + ".obj" << con::endl;
				temp_vertex.~vector(); temp_normal.~vector(); temp_texture.~vector();
				bEnable = false;
				throw RUNTIME_ERROR(MODEL_LOAD_ERROR);
			}
			meshPtr->VB->Lock(0, 0, (void**)&data_, 0);
			int data_count = 0;
			while (!feof(objfile))
			{
				fgets(read_line, sizeof(read_line), objfile);
				if (cStrFind(read_line, "f ") == 0)
				{
					int tmp[6];
					int before = 2;
					int count = 0;
					for (int i = 2;; i++)
					{
						if (read_line[i] < 48 || read_line[i] > 57)
						{
							if (before != i)
							{
								int out = 0;
								for (int j = before; j != i; j++)
								{
									out = out * 10 + read_line[j] - 48;
								}
								before = i + 1;
								tmp[count++] = out;
								if (count == 6)
								{
									data_[data_count++] = VertexXYZTEX(
										temp_vertex[tmp[0]].x, temp_vertex[tmp[0]].y, temp_vertex[tmp[0]].z,
										temp_texture[tmp[1]].u, temp_texture[tmp[1]].v);
									data_[data_count++] = VertexXYZTEX(
										temp_vertex[tmp[4]].x, temp_vertex[tmp[4]].y, temp_vertex[tmp[4]].z,
										temp_texture[tmp[5]].u, temp_texture[tmp[5]].v);
									data_[data_count++] = VertexXYZTEX(
										temp_vertex[tmp[2]].x, temp_vertex[tmp[2]].y, temp_vertex[tmp[2]].z,
										temp_texture[tmp[3]].u, temp_texture[tmp[3]].v);

									count = 4;
									tmp[2] = tmp[4]; tmp[3] = tmp[5];
								}
							}
							if (read_line[i] == '\n' || feof(objfile))
							{
								break;
							}
						}
					}
					read_line[0] = '\0';
				}
				else if (cStrFind(read_line, "usemtl ") == 0)
				{
					meshPtr->VB->Unlock();
					break;
				}
			}
			if (meshPtr == --mesh.end())
			{
				meshPtr->VB->Unlock();
				break;
			}
		}
		else if (meshPtr->FVF == VertexXYZ::FVF)
		{
			VertexXYZ* data_;
			gSystem.device->CreateVertexBuffer(meshPtr->faceCount * 3 * sizeof(VertexXYZ), D3DUSAGE_WRITEONLY, meshPtr->FVF, D3DPOOL_MANAGED, &meshPtr->VB, 0);
			if (meshPtr->VB == NULL)
			{
				gSystem.console << con::error << con::func << "CreateVertexBuffer() - failed" << con::endl;
				gSystem.console << con::error << con::func << "release model : " << model_name + ".obj" << con::endl;
				temp_vertex.~vector(); temp_normal.~vector(); temp_texture.~vector();
				bEnable = false;
				throw RUNTIME_ERROR(MODEL_LOAD_ERROR);
			}
			meshPtr->VB->Lock(0, 0, (void**)&data_, 0);
			int data_count = 0;
			while (!feof(objfile))
			{
				fgets(read_line, sizeof(read_line), objfile);
				if (cStrFind(read_line, "f ") == 0)
				{
					int tmp[3];
					int before = 2;
					int count = 0;
					for (int i = 2;; i++)
					{
						if (read_line[i] < 48 || read_line[i] > 57)
						{
							if (before != i)
							{
								int out = 0;
								for (int j = before; j != i; j++)
								{
									out = out * 10 + read_line[j] - 48;
								}
								before = i + 1;
								tmp[count++] = out;
								if (count == 3)
								{
									data_[data_count++] = VertexXYZ(
										temp_vertex[tmp[0]].x, temp_vertex[tmp[0]].y, temp_vertex[tmp[0]].z);
									data_[data_count++] = VertexXYZ(
										temp_vertex[tmp[2]].x, temp_vertex[tmp[2]].y, temp_vertex[tmp[2]].z);
									data_[data_count++] = VertexXYZ(
										temp_vertex[tmp[1]].x, temp_vertex[tmp[1]].y, temp_vertex[tmp[1]].z);

									count = 2;
									tmp[1] = tmp[2];
								}
							}
							if (read_line[i] == '\n' || feof(objfile))
							{
								break;
							}
						}
					}
					read_line[0] = '\0';
				}
				else if (cStrFind(read_line, "usemtl ") == 0)
				{
					meshPtr->VB->Unlock();
					break;
				}
			}
			if (meshPtr == --mesh.end())
			{
				meshPtr->VB->Unlock();
				break;
			}
		}
		else
		{
			gSystem.console << con::error << con::func << "there is no matching FVF" << con::endl;
			gSystem.console << con::error << con::func << "error FVF : " << (int)meshPtr->FVF << con::endl;
			gSystem.console << con::error << con::func << "release model : " << model_name + ".obj" << con::endl;
			temp_vertex.~vector(); temp_normal.~vector(); temp_texture.~vector();
			bEnable = false;
			throw RUNTIME_ERROR(MODEL_LOAD_ERROR);
		}
	}
	fclose(objfile);

	cout << "obj file read completed!" << endl;
	cout << "v : " << temp_vertex.size() << endl;
	cout << "n : " << temp_normal.size() << endl;
	cout << "t : " << temp_texture.size() << endl;
	for (auto p = mesh.begin(); p != mesh.end(); p++)
	{
		cout << endl;
		cout << "mesh f : " << p->faceCount << endl;
		cout << "mesh FVF : " << p->FVF << endl;
		cout << "mesh mtl : " << p->mtl._Ptr << endl;
		cout << "mesh name : " << *p->mtl->name << endl;
		cout << "mesh VB : " << p->VB << endl;
	}
	for (auto p = mtl.begin(); p != mtl.end(); p++)
	{
		cout << endl;
		cout << "mtl ambient : " << p->material.ambient.x << " " << p->material.ambient.y << " " << p->material.ambient.z << endl;
		cout << "mtl diffuse : " << p->material.diffuse.x << " " << p->material.diffuse.y << " " << p->material.diffuse.z << endl;
		cout << "mtl specular : " << p->material.specular.x << " " << p->material.specular.y << " " << p->material.specular.z << endl;
		cout << "mtl illum : " << p->illum << endl;
		cout << "mtl name : " << *p->name;
		cout << "mtl texture : " << p->texture << endl;
	}
	gSystem.console.RestoreFunction();
}
void MODEL::ReadFace(char* readLine)
{
	int count = -1;
	for (int i = 2; readLine[i] != '\n' && readLine[i] != '\0'; i++)
	{
		if (readLine[i] == ' ' && (readLine[i + 1] != '\n' && readLine[i + 1] != '\0'))
			count++;
	}
	mesh.back().faceCount += count;
}

void MODEL::ReadF(char* _readLine, DWORD _FVF, int* _data)
{
	int before = 2;
	int count = 0;
	for (int i = 2;; i++)
	{
		if (_readLine[i] < 48 || _readLine[i] > 57)
		{
			int out = 0;
			for (int j = before; j != i; j++)
			{
				out = out * 10 + _readLine[j] - 48;
			}
			before = i + 1;
			_data[count++] = out;
			if (_readLine[i] == '\n' || _readLine[i] == '\0')
				break;
		}
	}
}

void MODEL::ReadMtl(string _mtlFileName)
{
	_mtlFileName.pop_back();
	FILE* mtlfile = fopen(_mtlFileName.c_str(), "r");
	if (mtlfile == NULL)
	{
		char error_msg[] = "mtl file Read Error : ";
		bEnable = false;
		throw runtime_error(error_msg + _mtlFileName);
	}

	char readLine[128];
	char* end;
	while (!feof(mtlfile))
	{
		fgets(readLine, sizeof(readLine), mtlfile);
		//	for (int j = 0; j < 128; j++)
		//		cout << readData[j] << " : " << (int)readData[j] << endl;
		if (cStrFind(readLine, "newmtl ") == 0)
		{
			MTL tmp;
			tmp.name = new string(readLine + 7);
			tmp.texture = gSystem.default_texture;
			tmp.illum = false;
			tmp.material.ambient.x = 0.0f;
			tmp.material.ambient.y = 0.0f;
			tmp.material.ambient.z = 0.0f;
			tmp.material.ambient.w = 1.0f;

			tmp.material.diffuse.x = 0.0f;
			tmp.material.diffuse.y = 0.0f;
			tmp.material.diffuse.z = 0.0f;
			tmp.material.diffuse.w = 1.0f;

			tmp.material.specular.x = 0.0f;
			tmp.material.specular.y = 0.0f;
			tmp.material.specular.z = 0.0f;
			tmp.material.specular.w = 1.0f;
			mtl.push_back(tmp);
		}
		else if (cStrFind(readLine, "Ka ") == 0)
		{
			mtl.back().material.ambient.x = strtof(readLine + 3, &end);
			mtl.back().material.ambient.y = strtof(end, &end);
			mtl.back().material.ambient.z = strtof(end, NULL);
			mtl.back().material.ambient.w = 1.0f;
		}
		else if (cStrFind(readLine, "Kd ") == 0)
		{
			mtl.back().material.diffuse.x = strtof(readLine + 3, &end);
			mtl.back().material.diffuse.y = strtof(end, &end);
			mtl.back().material.diffuse.z = strtof(end, NULL);
			mtl.back().material.diffuse.w = 1.0f;
		}
		else if (cStrFind(readLine, "Ks ") == 0)
		{
			mtl.back().material.specular.x = strtof(readLine + 3, &end);
			mtl.back().material.specular.y = strtof(end, &end);
			mtl.back().material.specular.z = strtof(end, NULL);
			mtl.back().material.specular.w = 1.0f;
		}
		else if (cStrFind(readLine, "map_Kd ") == 0)
		{
			string textureFileName(readLine + 7);
			textureFileName.pop_back();
			gSystem.texture[*mtl.back().name] = NULL;
			if (SUCCEEDED(D3DXCreateTextureFromFile
			(gSystem.device, (path + textureFileName).c_str(), &gSystem.texture[*mtl.back().name])))
				mtl.back().texture = &gSystem.texture[*mtl.back().name];
		}
	}
	fclose(mtlfile);
}

void MODEL::ReadOrm()
{
}
void MODEL::Move(D3DXMATRIXA16 _position)
{
	position = _position;
}

void MODEL::Test()
{
	if (bEnable)
	{
		gSystem.shader->SetMatrix("gMatWorld", &position);

		for (auto ptr = mesh.begin(); ptr != mesh.end(); ptr++)
		{
			if (gSystem.savedFVF != ptr->FVF)
			{
				gSystem.device->SetFVF(ptr->FVF);
				gSystem.savedFVF = ptr->FVF;
			}
			if (ptr->FVF == VertexXYZTEXNOR::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZTEXNOR));

			else if (ptr->FVF == VertexXYZ::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZ));

			else if (ptr->FVF == VertexXYZNOR::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZNOR));

			else if (ptr->FVF == VertexXYZTEX::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZTEX));
			else
				cout << "error";

			gSystem.device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, ptr->faceCount);
		}
	}
}

void MODEL::ShadowRender()
{
	if (bEnable)
	{
		gSystem.shadow->SetMatrix("gMatWorld", &position);

		for (auto ptr = mesh.begin(); ptr != mesh.end(); ptr++)
		{
			if (gSystem.savedFVF != ptr->FVF)
			{
				gSystem.device->SetFVF(ptr->FVF);
				gSystem.savedFVF = ptr->FVF;
			}
			gSystem.shadow->SetTexture("gMainTexture", *(ptr->mtl->texture));
			gSystem.shadow->CommitChanges();

			if (ptr->FVF == VertexXYZTEXNOR::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZTEXNOR));

			else if (ptr->FVF == VertexXYZ::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZ));

			else if (ptr->FVF == VertexXYZNOR::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZNOR));

			else if (ptr->FVF == VertexXYZTEX::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZTEX));
			else
				cout << "error";
			gSystem.device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, ptr->faceCount);
		}
	}
}
void MODEL::RenderVertical()
{
	if (bEnable)
	{
		for (auto ptr = mesh.begin(); ptr != mesh.end(); ptr++)
		{
			if (gSystem.savedFVF != ptr->FVF)
			{
				gSystem.device->SetFVF(ptr->FVF);
				gSystem.savedFVF = ptr->FVF;
			}
			if (ptr->FVF == VertexXYZTEXNOR::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZTEXNOR));

			else if (ptr->FVF == VertexXYZ::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZ));

			else if (ptr->FVF == VertexXYZNOR::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZNOR));

			else if (ptr->FVF == VertexXYZTEX::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZTEX));
			else
				cout << "error";

			gSystem.device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, ptr->faceCount);
		}
	}
}
void MODEL::SoftShadow()
{
	if (bEnable)
	{
		gSystem.SoftShadow->SetMatrix("WorldMatrix", &position);

		for (auto ptr = mesh.begin(); ptr != mesh.end(); ptr++)
		{
			if (gSystem.savedFVF != ptr->FVF)
			{
				gSystem.device->SetFVF(ptr->FVF);
				gSystem.savedFVF = ptr->FVF;
			}
			gSystem.SoftShadow->SetTexture("MainTexture", *(ptr->mtl->texture));
			gSystem.SoftShadow->CommitChanges();

			if (ptr->FVF == VertexXYZTEXNOR::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZTEXNOR));

			else if (ptr->FVF == VertexXYZ::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZ));

			else if (ptr->FVF == VertexXYZNOR::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZNOR));

			else if (ptr->FVF == VertexXYZTEX::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZTEX));
			else
				cout << "error";

			gSystem.device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, ptr->faceCount);
		}
	}
}
void MODEL::test3()
{
	if (bEnable)
	{
		gSystem.MainShader->SetMatrix("WorldMatrix", &position);
		for (auto ptr = mesh.begin(); ptr != mesh.end(); ptr++)
		{
			if (gSystem.savedFVF != ptr->FVF)
			{
				gSystem.device->SetFVF(ptr->FVF);
				gSystem.savedFVF = ptr->FVF;
			}
			gSystem.MainShader->SetTexture("MainTexture", *(ptr->mtl->texture));
			gSystem.MainShader->CommitChanges();

			if (ptr->FVF == VertexXYZTEXNOR::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZTEXNOR));

			else if (ptr->FVF == VertexXYZ::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZ));

			else if (ptr->FVF == VertexXYZNOR::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZNOR));

			else if (ptr->FVF == VertexXYZTEX::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZTEX));
			else
				cout << "error";

			gSystem.device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, ptr->faceCount);
		}
	}
}

void MODEL::Render()
{
	if (bEnable)
	{
		gSystem.shader->SetMatrix("gMatWorld", &position);

		for (auto ptr = mesh.begin(); ptr != mesh.end(); ptr++)
		{
			if (gSystem.savedFVF != ptr->FVF)
			{
				gSystem.device->SetFVF(ptr->FVF);
				gSystem.savedFVF = ptr->FVF;
			}

			gSystem.shader->SetTechnique("VTN");

			gSystem.shader->SetVector("Ambient", &ptr->mtl->material.ambient);
			gSystem.shader->SetVector("Diffuse", &ptr->mtl->material.diffuse);
			gSystem.shader->SetVector("Specular", &ptr->mtl->material.specular);
			gSystem.shader->SetTexture("DiffuseTexture", *(ptr->mtl->texture));
			gSystem.shader->CommitChanges();

			if (ptr->FVF == VertexXYZTEXNOR::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZTEXNOR));

			else if (ptr->FVF == VertexXYZ::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZ));

			else if (ptr->FVF == VertexXYZNOR::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZNOR));

			else if (ptr->FVF == VertexXYZTEX::FVF)
				gSystem.device->SetStreamSource(0, ptr->VB, 0, sizeof(VertexXYZTEX));
			else
				cout << "error";
		
			gSystem.device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, ptr->faceCount);
		}
	}
}
void MODEL::Release()
{
	if (!mesh.empty())
	{
		for (auto ptr = mesh.begin(); ptr != mesh.end(); ptr++)
		{
			Release_<IDirect3DVertexBuffer9*>(ptr->VB);
		}

	}
	if (!mtl.empty())
	{
		for (auto ptr = mtl.begin(); ptr != mtl.end(); ptr++)
		{
			if (ptr->name != NULL)
				delete ptr->name;
		}
	}
}