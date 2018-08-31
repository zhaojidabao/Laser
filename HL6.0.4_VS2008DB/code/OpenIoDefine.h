//******************************************************************//
//                      OpenIo v1.0 驱动程序配套动态库              //
//    可使Ring3级进程在NT/2000/XP操作系统平台上直接进行IO操作。     //
//    使用时在CApp类的InitInstance()函数的最开始调用InitializeOpenIO//
//    函数，并在CApp类的ExitInstance()函数的最后调用ShutdownOpenIo  //
//    函数即可，                                                    //
//                                                                  //
//                程序设计：朱刚                                    //
//               修改日期：2004年2月25日                            //
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