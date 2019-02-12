#include <stdlib.h>
#include <string.h>

#include "simplepng.h"

bitmap_t simple_png_read(char* file_name)
{
  bitmap_t result = {
    .width = 0,
    .height = 0,
    .color_type = 0,
    .bit_depth = 0,
    .rows = NULL,
    .row_count = 0
  };
  FILE *fd;
  unsigned char sig[8];
  png_structp png_ptr;
  png_infop info_ptr;
  int y;
  int number_of_passes;

  /* open file */
  if (strcmp(file_name, "-") == 0)
  {
    fd = stdin;
  }
  else
  {
    fd = fopen(file_name, "rb");
    if (!fd)
    {
      perror("fopen");
      return result;
    }
  }

  /* read and compare signature */
  if(fread(sig, 1, 8, fd) < 8)
  {
    perror("fread");
    return result;
  }
  if(png_sig_cmp(sig, 0, 8))
  {
    printf("sig\n");
    return result;
  }

  //utwórz wymagane struktury danych
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png_ptr)
  {
    printf("read struct\n");
    return result;
  }
  info_ptr = png_create_info_struct(png_ptr);
  if(!info_ptr)
  {
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    printf("info struct\n");
    return result;
  }
  setjmp(png_jmpbuf(png_ptr));

  png_set_sig_bytes(png_ptr, 8);
  png_init_io(png_ptr, fd);
  //png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 |
  //    PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);
  png_read_info(png_ptr, info_ptr);

  result.width = png_get_image_width(png_ptr, info_ptr);
  result.height = png_get_image_height(png_ptr, info_ptr);
  result.color_type = png_get_color_type(png_ptr, info_ptr);
  result.bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  //result.row_count = png_get_rowbytes(png_ptr, info_ptr);
  //result.rows = png_get_rows(png_ptr, info_ptr);

  number_of_passes = png_set_interlace_handling(png_ptr);
  png_read_update_info(png_ptr, info_ptr);

  /* allocate rows */
  result.rows = (png_bytep*) malloc(sizeof(png_bytep) * result.height);
  for (y=0; y<result.height; y++)
  {
    result.row_count = png_get_rowbytes(png_ptr,info_ptr);
    result.rows[y] = (png_byte*) malloc(result.row_count);
  }

  /* read whole PNG */
  png_read_image(png_ptr, result.rows);

  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

  fclose(fd);

  return result;
}

void simple_png_free(bitmap_t *bmp)
{
  int y;
  for (y = 0; y < bmp->height; y++)
  {
    free(bmp->rows[y]);
  }
  free(bmp->rows);
}
