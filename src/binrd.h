#ifndef BINRD_H
#define BINRD_H
#include <stdint.h>

typedef struct {
  uint8_t *stream; /**< pointer to next unprocessed byte in bitstream */
  int bitcount; /**< number of bits left to process */
  uint8_t bits;
  int bits_loaded;
} bitstream_t;

/**
 * \brief Initialize bitstream parsing state structure
 * \details First bit in bitstream should be most significant bit of first byte.
 * After last bit array should be aligned to full byte
 *
 * \param buf Pointer to buffer with bitstream to process
 * \param bitcount Number of bits in bitstream
 *
 * \return Structure describing parsing state
 */
bitstream_t init_bitstream(uint8_t *buf, int bitcount);

/**
 * \brief Get next bit from stream
 *
 * \param bs Pointer to bitstream descriptor
 *
 * \return Read bit
 * \retval 1 Bit was set
 * \retval 0 Bit was clear
 */
int get_bit(bitstream_t *bs);

/**
 * \brief Checks if there is any bit left in stream
 *
 * \param bs Pointer to bitstream descriptor
 *
 * \return True if at least one bit left
 * \retval 1 One or more bits left
 * \retval 0 No more bits left
 */
int isnext(bitstream_t *bs);

#endif // BINRD_H
