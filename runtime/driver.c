#ifdef PLATFORM_WINDOWS
  #include<Windows.h>
#endif
#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/time.h>
#include<errno.h>

//     Stanza Defined Entities
//     =======================
int64_t stanza_entry (char* stack_mem);
extern int64_t stanza_stack_size;

//     Command line arguments
//     ======================
int64_t input_argc;
char** input_argv;

//     Main Driver
//     ===========
int main (int argc, char* argv[]) {  
  input_argc = argc;
  input_argv = argv;
  char* stack_mem = (char*)malloc(stanza_stack_size);
  stanza_entry(stack_mem);
  return 0;
}

//     Macro Readers
//     =============
FILE* get_stdout () {return stdout;}
FILE* get_stderr () {return stderr;}
FILE* get_stdin () {return stdin;}
int get_eof () {return EOF;}
int get_errno () {return errno;}

//     Time of Day
//     ===========
int64_t current_time_us (void) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (int64_t)tv.tv_sec * 1000 * 1000 + (int64_t)tv.tv_usec;
}

int64_t current_time_ms (void) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (int64_t)tv.tv_sec * 1000 + (int64_t)tv.tv_usec / 1000;
}

//     Path Resolution
//     ===============
#ifdef PLATFORM_WINDOWS
  int file_exists (char* filename) {
    int attrib = GetFileAttributes(filename);
    return attrib != INVALID_FILE_ATTRIBUTES;
  }

  char* resolve_path (char* filename){
    if(file_exists(filename)){
      char* fileext;
      char* path = (char*)malloc(2048);
      int ret = GetFullPathName(filename, 2048, path, &fileext);
      if(ret == 0){
        free(path);
        return 0;
      }else{
        return path;
      }             
    }
    else{
      return 0;
    }
  }
#else
  char* realpath(const char *path, char *resolved_path);
  char* resolve_path (char* filename){
    return realpath(filename, 0);
  }
#endif

//     Environment Variable Setting
//     ============================
#ifdef PLATFORM_WINDOWS
  int setenv (char* name, char* value, int overwrite) {
    //If we don't want to overwrite previous value, then check whether it exists.
    //If it does, then just return 0.
    if(!overwrite){
      if(getenv(name) == 0)
        return 0;
    }
    //(Over)write the environment variable.
    char* buffer = (char*)malloc(strlen(name) + strlen(value) + 10);
    sprintf(buffer, "%s=%s", name, value);
    _putenv(buffer);
    free(buffer);
    return 0;
  }
#endif
