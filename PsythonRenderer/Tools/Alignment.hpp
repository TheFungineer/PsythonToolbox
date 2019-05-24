/*/
 * Use of this source code is governed by an MIT-style
 * license that can be found in the LICENSE file or at
 * https://opensource.org/licenses/MIT
/*/
#pragma once

namespace psy {

inline ptrdiff_t paddingFor(intptr_t inAddress, intptr_t inAlignment)
{
	return static_cast<ptrdiff_t>(-inAddress & (inAlignment - 1));
}

inline bool isAligned(intptr_t inAddress, intptr_t inAlignment)
{
	return !(inAddress & (inAlignment - 1));
}
	
} // namespace psy
