//******************************************************************//
//                      OpenIo v1.0 �����������׶�̬��              //
//    ��ʹRing3��������NT/2000/XP����ϵͳƽ̨��ֱ�ӽ���IO������     //
//    ʹ��ʱ��CApp���InitInstance()�������ʼ����InitializeOpenIO//
//    ����������CApp���ExitInstance()������������ShutdownOpenIo  //
//    �������ɣ�                                                    //
//                                                                  //
//                ������ƣ����                                    //
//               �޸����ڣ�2004��2��25��                            //
//                                                                  //
//******************************************************************//


#ifndef OPENIO_H
#define OPENIO_H

#ifdef OPENIO_DLL
#define OPENIO_API extern "C" __declspec(dllexport)
#else
#define OPENIO_API extern "C" __declspec(dllimport)
#endif

OPENIO_API BOOL _stdcall InitializeOpenIo();
OPENIO_API void _stdcall ShutdownOpenIo();


#endif