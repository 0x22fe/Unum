#include "Unum.h"

int main(int argc, char** argv)
{
    unum_log("Unum Interpreter (Version Code %u, Built on " __DATE__ ")", UNUM_VERSION);

    if(argc != 2)
    {
        printf("Unum Interpreter (Version Code %u, Built on " __DATE__ ")\n", UNUM_VERSION);
        printf("Usage: unum [filename]\n");
        return -1;
    } else {

        // Refer to https://stackoverflow.com/a/3747128
        // Read file
        FILE *f;
        size len;
        str buf;

        f = fopen (argv[1], "rb");
        if(!f)
        {
            printf("Failed to read file \"%s\", I/O error\n", argv[1]);
            return -1;
        }

        // Get file size
        fseek(f, 0L, SEEK_END);
        len = ftell(f);
        rewind(f);

        // Allocate memory
        buf = UNUM_MALLOC(sizeof(txt) * (len + 1));
        if(!buf)
        {
            fclose(f);
            printf("Failed to read file \"%s\", memory error\n", argv[1]);
            return -1;
        }

        // Copy file
        if(fread(buf, len, 1, f) != 1)
        {
            fclose(f);
            free(buf);
            printf("Failed to read file \"%s\", copy error\n", argv[1]);
            return -1;
        }

        // Close file
        fclose(f);

        // Terminate string
        buf[len] = '\0';

        // Print contents
        // printf("File: \"%s\"\n", buf);

        // Create program
        UnumInstance* ci = Unum_Initialize();

        if(ci == NULL)
            return -1;

        // Execute program
        i32 result = Unum_Execute(ci, buf);

        // Free code
        free(buf);

        // Destroy program
        Unum_Destroy(ci);

        // Return program result
        return (int)(result);
    }
}
