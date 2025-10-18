#include "Application.hpp"

int main()
{
	Application app("Learn OpenGL");
	app.Run();
}

//void RenderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color)
//{
//	// activate corresponding shader
//	s.use();
//	s.setVec3("textColor", color);
//	glActiveTexture(GL_TEXTURE0);
//	glBindVertexArray(VAO2);
//	glEnable(GL_BLEND);
//	glEnable(GL_CULL_FACE);
//
//	// iretate through all characters
//	std::string::const_iterator c;
//	for (c = text.begin(); c != text.end(); ++c)
//	{
//		Character ch = Characters[*c];
//
//		float xpos = x + ch.Bearing.x * scale;
//		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
//
//		float w = ch.Size.x * scale;
//		float h = ch.Size.y * scale;
//		// update VBO for each chracter
//		float vertices[6][4] = {
//			{xpos, ypos + h, 0.0f, 0.0f},
//			{xpos, ypos, 0.0f, 1.0f},
//			{xpos + w, ypos, 1.0f, 1.0f},
//
//			{xpos, ypos + h, 0.0f, 0.0f},
//			{xpos + w, ypos, 1.0f, 1.0f},
//			{xpos + w, ypos + h, 1.0f, 0.0f}
//		};
//		// render the glyph texture over quad
//		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
//		// update the content of the VBO memory
//		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
//		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		// render quad
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
//		x += (ch.Advance >> 6) * scale; // bit shift by 6 to get value in pixels (2^6 = 64)
//	}
//	glBindVertexArray(0);
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glDisable(GL_BLEND);
//	glDisable(GL_CULL_FACE);
//}
