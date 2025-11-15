#include "VBO.h"
#include <iostream>

void VBO::UpdateData(const void* data, GLsizeiptr size, GLintptr offset)
{
	glBindBuffer(m_Target, m_ID);


	if (size != m_Size) {
		// If the size is different, we need to reallocate the buffer
		glBufferData(m_Target, size, nullptr, m_DrawMode); // Reallocate buffer with new size

		m_Size = size; // Update the size
	}
	glBufferSubData(m_Target, offset, size, data);

}

void VBO::Bind()
{
	glBindBuffer(m_Target, m_ID); //bind VBO
}

void VBO::Unbind()
{
	glBindBuffer(m_Target, 0); //bind VBO
}

void VBO::Delete()
{
	glDeleteBuffers(1, &m_ID);
}
