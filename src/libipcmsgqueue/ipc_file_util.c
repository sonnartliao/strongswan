#ifndef _IPC_FILE_UTIL_H_
#define _IPC_FILE_UTIL_H_
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "ipc_msg_queue_comm.h"
#include "ipc_file_util.h"

#define MAX_ROW_COUNT 128
#define MAX_COL_COUNT 128
#define MAX_PATH_BUFF_LEN 1024

bool create_file_if_absent(const char *pFullFileName)
{
  IPC_CHECK_PTR_RV(pFullFileName, false);
  IPC_CHECK_EXPR_RV(strlen(pFullFileName) > 0 && strlen(pFullFileName) < MAX_PATH_BUFF_LEN, false);

  char arrFullPath[MAX_PATH_BUFF_LEN] = "";
  char arrPath[MAX_PATH_BUFF_LEN] = "";
  char arrCmd[2 * MAX_PATH_BUFF_LEN] = "";
  char arrBuffers[MAX_ROW_COUNT][MAX_COL_COUNT];
  const char *delimiter = "/";
  char *token;
  int nLines;
  int i;

  ZERO_ARRAY(arrPath);
  ZERO_ARRAY(arrBuffers);

  strcpy(arrFullPath, pFullFileName);

  token = strtok(arrFullPath, delimiter);
  nLines = 0;

  while (token != NULL)
  {
    if (strlen(token) > MAX_COL_COUNT)
    {
      println("exceeds the maximum length %d", MAX_COL_COUNT);
      return false;
    }

    if (nLines >= MAX_ROW_COUNT)
    {
      println("exceeds the maximum elements %d", nLines);
      return false;
    }

    strncpy(arrBuffers[nLines], token, MAX_COL_COUNT - 1);
    nLines++;
    token = strtok(NULL, delimiter);
  }

  if (nLines < 1)
  {
    println("invalid full file name %s", pFullFileName);
    return false;
  }

  for (i = 0; i < nLines - 1; i++)
  {
    if (strlen(arrPath) + strlen(arrBuffers[i] + 1) >= MAX_PATH_BUFF_LEN)
    {
      println("path is too long %lu", strlen(pFullFileName));
      return false;
    }

    strcat(arrPath, "/");
    strcat(arrPath, arrBuffers[i]);
  }

  if (strlen(arrPath) == 0)
  {
    println("invalid path %s", arrPath);
    return false;
  }

  ZERO_ARRAY(arrCmd);
  if (access(arrPath, F_OK) != 0)
  {
    println("path %s is not exist,create it", arrPath);
    strcat(arrCmd, "mkdir -p ");
    strcat(arrCmd, arrPath);
    system(arrCmd);

    ZERO_ARRAY(arrCmd);
    strcat(arrCmd, "chmod -R 755 ");
    strcat(arrCmd, arrPath);
    system(arrCmd);
  }

  if (access(pFullFileName, F_OK) != 0)
  {
    ZERO_ARRAY(arrCmd);
    println("file %s is not exist ,create it", pFullFileName);
    strcat(arrCmd, "touch ");
    strcat(arrCmd, pFullFileName);
    system(arrCmd);
  }

  return true;
}

#endif