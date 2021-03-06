/*
  libbuf - a C library that provides fast, flexible and efficient buffers

  Copyright (C) 2013 onwards  Attie Grande (attie@attie.co.uk)

  libbuf is free software: you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  libbuf is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with libbuf. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal.h"

EXPORT size_t buf_takeStart(buf_t *buf, int flags, uint8_t **data, size_t count) {
	buf_chunk_t **p;
	size_t ret;

	if (!buf || !data) {
		errno = EINVAL;
		return -1;
	}
	if (count == 0) return 0;

	pthread_mutex_lock(&(buf->mutex));

	if (!(flags & BUF_NONBLOCK)) {
		/* wait for data */
		while (!buf->head || buf->head->len == 0) {
			if (_buf_wait(buf) != 0) {
				pthread_mutex_unlock(&(buf->mutex));
				return -1;
			}
		}
	}

	ret = _buf_get_data(buf, NULL, data, count);

	pthread_mutex_unlock(&(buf->mutex));

	return ret;
}

EXPORT size_t buf_takeFinish(buf_t *buf, size_t count) {
	buf_chunk_t **p;
	size_t ret;

	if (!buf) {
		errno = EINVAL;
		return -1;
	}
	if (count == 0) return 0;

	pthread_mutex_lock(&(buf->mutex));

	ret = _buf_takeFinish(buf, count);

	pthread_mutex_unlock(&(buf->mutex));

	return ret;
}
