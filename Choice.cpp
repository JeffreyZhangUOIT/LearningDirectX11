#include "Choice.h"

Choice::Choice(Renderer& ren) {
	// Initalise vertex and index buffers
	// Create our vertext buffer
	auto vertexBufferDesc = CD3D11_BUFFER_DESC(sizeof(box1), D3D11_BIND_VERTEX_BUFFER);
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA vertexData = { 0 };
	vertexData.pSysMem = box1;

	ren.getDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVertexBuffer);

	DWORD indices[6] =
	{
		0, 1, 2,
		3, 2, 1
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = sizeof(DWORD) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Specify the data to initialize the index buffer.
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;

	// Create the index buffer.
	ren.getDevice()->CreateBuffer(&ibd, &iinitData, &mIB);
	ren.getDeviceContext()->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	uninitDevice = false;
}
Choice::~Choice() {

}

void Choice::init(char param[1000]){
	madeChoice = false;
	numOfChoices = 0;
	selection = 0;
	char * token;
	char * nextTok;
	int i = 0;
	char temp[1000];
	memset(sceneBuffer, '\0', sizeof(sceneBuffer));
	memset(choiceBuffer, '\0', sizeof(choiceBuffer));
	memset(choice1, '\0', sizeof(choice1));
	memset(choice2, '\0', sizeof(choice2));
	memset(choice3, '\0', sizeof(choice3));
	memset(choice4, '\0', sizeof(choice4));

	token = strtok_s(param, "{}", &nextTok);
	while (*token)
	{
		choiceBuffer[i] = *token;
		token++;
		i++;
	}
	i = 0;
	token = strtok_s(NULL, "{}", &nextTok);
	while (*token)
	{
		sceneBuffer[i] = *token;
		token++;
		i++;
	}
	i = 0;

	memcpy(temp, choiceBuffer, sizeof(temp));
	token = strtok_s(temp, ",\0", &nextTok);

	while (token != NULL) {
		if (numOfChoices == 0) {
			while ((*token))
			{
				choice1[i] = *token;
				token++;
				i++;
			}
		}
		if (numOfChoices == 1) {
			while ((*token) )
			{
				choice2[i] = *token;
				token++;
				i++;
			}
		}
		if (numOfChoices == 2) {
			while ((*token))
			{
				choice3[i] = *token;
				token++;
				i++;
			}
		}
		if (numOfChoices == 3) {
			while ((*token))
			{
				choice4[i] = *token;
				token++;
				i++;
			}
		}
		i = 0;
		numOfChoices++;
		token = strtok_s(NULL, ",\0", &nextTok);
	}

}

void Choice::draw(Renderer& ren, TextureManager& tex) 
{
	auto deviceContext = ren.getDeviceContext();

	tex.changeTex(ren, 15);
	
	// Bind our Player shaders
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Bind our vertex buffer
	UINT stride = sizeof(Renderer::Vertex);
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	// Draw
	deviceContext->DrawIndexed(6, 0, 0);
}

void Choice::drawText(TextHandler& text) {
	// Draw text on top of decision box.
	size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, choice1, -1, NULL, 0);
	buffer = new wchar_t[size];
	if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, choice1, -1, buffer, size)) {
		exit(99);
	}
	text.drawMyText(buffer, (box1[0].x + box1[3].x) / 2, (box1[0].y + box1[3].y) / 2, { 1, 0, 0, 1 });
	delete buffer;

	if (numOfChoices > 1) {
		// Draw text on top of decision box.
		size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, choice2, -1, NULL, 0);
		buffer = new wchar_t[size];
		if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, choice2, -1, buffer, size)) {
			exit(99);
		}
		text.drawMyText(buffer, (box2[0].x + box2[3].x) / 2, (box2[0].y + box2[3].y) / 2, {1, 0, 0, 1 });
		delete buffer;
	}

	if (numOfChoices > 2) {
		// Draw text on top of decision box.
		size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, choice3, -1, NULL, 0);
		buffer = new wchar_t[size];
		if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, choice3, -1, buffer, size)) {
			exit(99);
		}
		text.drawMyText(buffer, (box3[0].x + box3[3].x) / 2, (box3[0].y + box3[3].y) / 2, {1, 0, 0, 1 });
		delete buffer;
	}

	if (numOfChoices > 3) {
		// Draw text on top of decision box.
		size = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, choice4, -1, NULL, 0);
		buffer = new wchar_t[size];
		if (!MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, choice4, -1, buffer, size)) {
			exit(99);
		}
		text.drawMyText(buffer, (box4[0].x + box4[3].x) / 2, (box4[0].y + box4[3].y) / 2, { 1, 0, 0, 1 });
		delete buffer;
	}
}

void Choice::update(Renderer& ren, TextureManager& tex, float x, float y)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	point[0] = x; point[1] = y;

	if (numOfChoices == 1) 
	{

		// Setup and draw decision box.
		box1[0].x = -0.5f;
		box1[0].y = 0.1f;
		box1[0].Xtex = 0.0f;
		box1[0].Ytex = 0.0f;

		box1[1].x = 0.5f;
		box1[1].y = 0.1f; 
		box1[1].Xtex = 1.0f; 
		box1[1].Ytex = 0.0f;

		box1[2].x = -0.5f;
		box1[2].y = -0.1f;
		box1[2].Xtex = 0.0f;
		box1[2].Ytex = 1.0f;

		box1[3].x = 0.5f;
		box1[3].y = -0.1f;
		box1[3].Xtex = 1.0f;
		box1[3].Ytex = 1.0f;

		if ((point[0] > box1[0].x) & (point[1] > box1[3].y) & (point[0] < box1[3].x) & (point[1] < box1[0].y))
		{
			box1[0].Ytex = 0.5f;
			box1[1].Ytex = 0.5f;
		}
		else
		{
			box1[2].Ytex = 0.5f;
			box1[3].Ytex = 0.5f;
		}

		ren.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, box1, sizeof(box1));
		ren.getDeviceContext()->Unmap(m_pVertexBuffer, 0);
		
		draw(ren, tex);
		if ((point[0] > box1[0].x) & (point[1] > box1[3].y) & (point[0] < box1[3].x) & (point[1] < box1[0].y) & ((GetKeyState(VK_LBUTTON) & 0x80) != 0)) {
			madeChoice = true;
			selection = 1;
		}


	}
	if (numOfChoices == 2)
	{
		box1[0].x = -0.5f;
		box1[0].y = 0.3f;
		box1[0].Xtex = 0.0f;
		box1[0].Ytex = 0.0f;

		box1[1].x = 0.5f;
		box1[1].y = 0.3f;
		box1[1].Xtex = 1.0f;
		box1[1].Ytex = 0.0f;

		box1[2].x = -0.5f;
		box1[2].y = 0.1f;
		box1[2].Xtex = 0.0f;
		box1[2].Ytex = 1.0f;

		box1[3].x = 0.5f;
		box1[3].y = 0.1f;
		box1[3].Xtex = 1.0f;
		box1[3].Ytex = 1.0f;

	
		if ((point[0] > box1[0].x) & (point[1] > box1[3].y) & (point[0] < box1[3].x) & (point[1] < box1[0].y))
		{
			box1[0].Ytex = 0.5f;
			box1[1].Ytex = 0.5f;
		}
		else
		{
			box1[2].Ytex = 0.5f;
			box1[3].Ytex = 0.5f;
		}

		ren.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, box1, sizeof(box1));
		ren.getDeviceContext()->Unmap(m_pVertexBuffer, 0);
		draw(ren, tex);

		if ((point[0] > box1[0].x) & (point[1] > box1[3].y) & (point[0] < box1[3].x) & (point[1] < box1[0].y) & ((GetKeyState(VK_LBUTTON) & 0x80) != 0)) {
			madeChoice = true;
			selection = 1;
		}

		box2[0].x = -0.5f;
		box2[0].y = 0.0f;
		box2[0].Xtex = 0.0f;
		box2[0].Ytex = 0.0f;

		box2[1].x = 0.5f;
		box2[1].y = 0.0f;
		box2[1].Xtex = 1.0f;
		box2[1].Ytex = 0.0f;

		box2[2].x = -0.5f;
		box2[2].y = -0.2f;
		box2[2].Xtex = 0.0f;
		box2[2].Ytex = 1.0f;

		box2[3].x = 0.5f;
		box2[3].y = -0.2f;
		box2[3].Xtex = 1.0f;
		box2[3].Ytex = 1.0f;

		if ((point[0] > box2[0].x) & (point[1] > box2[3].y) & (point[0] < box2[3].x) & (point[1] < box2[0].y))
		{
			box2[0].Ytex = 0.5f;
			box2[1].Ytex = 0.5f;
		}
		else
		{
			box2[2].Ytex = 0.5f;
			box2[3].Ytex = 0.5f;
		}

		ren.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, box2, sizeof(box2));
		ren.getDeviceContext()->Unmap(m_pVertexBuffer, 0);
		draw(ren, tex);

		if ((point[0] > box2[0].x) & (point[1] > box2[3].y) & (point[0] < box2[3].x) & (point[1] < box2[0].y) &  ((GetKeyState(VK_LBUTTON) & 0x80) != 0)) {
			madeChoice = true;
			selection = 2;
		}
	}
	if (numOfChoices == 3)
	{
		// Setup and draw decision box.
		box1[0].x = -0.5f;
		box1[0].y = 0.4f;
		box1[0].Xtex = 0.0f;
		box1[0].Ytex = 0.0f;

		box1[1].x = 0.5f;
		box1[1].y = 0.4f;
		box1[1].Xtex = 1.0f;
		box1[1].Ytex = 0.0f;

		box1[2].x = -0.5f;
		box1[2].y = 0.2f;
		box1[2].Xtex = 0.0f;
		box1[2].Ytex = 1.0f;

		box1[3].x = 0.5f;
		box1[3].y = 0.2f;
		box1[3].Xtex = 1.0f;
		box1[3].Ytex = 1.0f;
		
		if ((point[0] > box1[0].x) & (point[1] > box1[3].y) & (point[0] < box1[3].x) & (point[1] < box1[0].y))
		{
			box1[0].Ytex = 0.5f;
			box1[1].Ytex = 0.5f;
		}
		else
		{
			box1[2].Ytex = 0.5f;
			box1[3].Ytex = 0.5f;

		}

		ren.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, box1, sizeof(box1));
		ren.getDeviceContext()->Unmap(m_pVertexBuffer, 0);
	
		draw(ren, tex);
		if ((point[0] > box1[0].x) & (point[1] > box1[3].y) & (point[0] < box1[3].x) & (point[1] < box1[0].y) & ((GetKeyState(VK_LBUTTON) & 0x80) != 0)) {
			madeChoice = true;
			selection = 1;
		}

		// Setup and draw decision box.
		box2[0].x = -0.5f;
		box2[0].y = 0.1f;
		box2[0].Xtex = 0.0f;
		box2[0].Ytex = 0.0f;

		box2[1].x = 0.5f;
		box2[1].y = 0.1f;
		box2[1].Xtex = 1.0f;
		box2[1].Ytex = 0.0f;

		box2[2].x = -0.5f;
		box2[2].y = -0.1f;
		box2[2].Xtex = 0.0f;
		box2[2].Ytex = 1.0f;

		box2[3].x = 0.5f;
		box2[3].y = -0.1f;
		box2[3].Xtex = 1.0f;
		box2[3].Ytex = 1.0f;

		if ((point[0] > box2[0].x) & (point[1] > box2[3].y) & (point[0] < box2[3].x) & (point[1] < box2[0].y))
		{
			box2[0].Ytex = 0.5f;
			box2[1].Ytex = 0.5f;
		}
		else
		{
			box2[2].Ytex = 0.5f;
			box2[3].Ytex = 0.5f;
		}

		ren.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, box2, sizeof(box2));
		ren.getDeviceContext()->Unmap(m_pVertexBuffer, 0);
		
		draw(ren, tex);

		if ((point[0] > box2[0].x) & (point[1] > box2[3].y) & (point[0] < box2[3].x) & (point[1] < box2[0].y) &  ((GetKeyState(VK_LBUTTON) & 0x80) != 0)) {
			madeChoice = true;
			selection = 2;
		}

		// Setup and draw decision box.
		box3[0].x = -0.5f;
		box3[0].y = -0.2f;
		box3[0].Xtex = 0.0f;
		box3[0].Ytex = 0.0f;

		box3[1].x = 0.5f;
		box3[1].y = -0.2f;
		box3[1].Xtex = 1.0f;
		box3[1].Ytex = 0.0f;

		box3[2].x = -0.5f;
		box3[2].y = -0.4f;
		box3[2].Xtex = 0.0f;
		box3[2].Ytex = 1.0f;

		box3[3].x = 0.5f;
		box3[3].y = -0.4f;
		box3[3].Xtex = 1.0f;
		box3[3].Ytex = 1.0f;

		if ((point[0] > box3[0].x) & (point[1] > box3[3].y) & (point[0] < box3[3].x) & (point[1] < box3[0].y))
		{
			box3[0].Ytex = 0.5f;
			box3[1].Ytex = 0.5f;
		}
		else
		{
			box3[2].Ytex = 0.5f;
			box3[3].Ytex = 0.5f;
		}

		ren.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, box3, sizeof(box3));
		ren.getDeviceContext()->Unmap(m_pVertexBuffer, 0);
		
		draw(ren, tex);

		if ((point[0] > box3[0].x) & (point[1] > box3[3].y) & (point[0] < box3[3].x) & (point[1] < box3[0].y) & ((GetKeyState(VK_LBUTTON) & 0x80) != 0)) {
			madeChoice = true;
			selection = 3;
		}
	}

	if (numOfChoices == 4)
	{
		// Setup and draw decision box.
		box1[0].x = -0.5f;
		box1[0].y = 0.6f;
		box1[0].Xtex = 0.0f;
		box1[0].Ytex = 0.0f;

		box1[1].x = 0.5f;
		box1[1].y = 0.6f;
		box1[1].Xtex = 1.0f;
		box1[1].Ytex = 0.0f;

		box1[2].x = -0.5f;
		box1[2].y = 0.4f;
		box1[2].Xtex = 0.0f;
		box1[2].Ytex = 1.0f;

		box1[3].x = 0.5f;
		box1[3].y = 0.4f;
		box1[3].Xtex = 1.0f;
		box1[3].Ytex = 1.0f;

		if ((point[0] > box1[0].x) & (point[1] > box1[3].y) & (point[0] < box1[3].x) & (point[1] < box1[0].y))
		{
			box1[0].Ytex = 0.5f;
			box1[1].Ytex = 0.5f;
		}
		else
		{
			box1[2].Ytex = 0.5f;
			box1[3].Ytex = 0.5f;
		}

		ren.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, box1, sizeof(box1));
		ren.getDeviceContext()->Unmap(m_pVertexBuffer, 0);
		
		draw(ren, tex);

		if ((point[0] > box1[0].x) & (point[1] > box1[3].y) & (point[0] < box1[3].x) & (point[1] < box1[0].y) & ((GetKeyState(VK_LBUTTON) & 0x80) != 0)) {
			madeChoice = true;
			selection = 1;
		}
	
		// Setup and draw decision box.
		box2[0].x = -0.5f;
		box2[0].y = 0.3f;
		box2[0].Xtex = 0.0f;
		box2[0].Ytex = 0.0f;

		box2[1].x = 0.5f;
		box2[1].y = 0.3f;
		box2[1].Xtex = 1.0f;
		box2[1].Ytex = 0.0f;

		box2[2].x = -0.5f;
		box2[2].y = 0.1f;
		box2[2].Xtex = 0.0f;
		box2[2].Ytex = 1.0f;

		box2[3].x = 0.5f;
		box2[3].y = 0.1f;
		box2[3].Xtex = 1.0f;
		box2[3].Ytex = 1.0f;

		if ((point[0] > box2[0].x) & (point[1] > box2[3].y) & (point[0] < box2[3].x) & (point[1] < box2[0].y))
		{
			box2[0].Ytex = 0.5f;
			box2[1].Ytex = 0.5f;
		}
		else
		{
			box2[2].Ytex = 0.5f;
			box2[3].Ytex = 0.5f;
		}

		ren.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, box2, sizeof(box2));
		ren.getDeviceContext()->Unmap(m_pVertexBuffer, 0);
		
		draw(ren, tex);

		if ((point[0] > box2[0].x) & (point[1] > box2[3].y) & (point[0] < box2[3].x) & (point[1] < box2[0].y) & ((GetKeyState(VK_LBUTTON) & 0x80) != 0)) {
			madeChoice = true;
			selection = 2;
		}

		// Setup and draw decision box.
		box3[0].x = -0.5f;
		box3[0].y = 0;
		box3[0].Xtex = 0.0f;
		box3[0].Ytex = 0.0f;

		box3[1].x = 0.5f;
		box3[1].y = 0;
		box3[1].Xtex = 1.0f;
		box3[1].Ytex = 0.0f;

		box3[2].x = -0.5f;
		box3[2].y = -0.2f;
		box3[2].Xtex = 0.0f;
		box3[2].Ytex = 1.0f;

		box3[3].x = 0.5f;
		box3[3].y = -0.2f;
		box3[3].Xtex = 1.0f;
		box3[3].Ytex = 1.0f;

		if ((point[0] > box3[0].x) & (point[1] > box3[3].y) & (point[0] < box3[3].x) & (point[1] < box3[0].y))
		{
			box3[0].Ytex = 0.5f;
			box3[1].Ytex = 0.5f;
		}
		else
		{
			box3[2].Ytex = 0.5f;
			box3[3].Ytex = 0.5f;
		}

		ren.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, box3, sizeof(box3));
		ren.getDeviceContext()->Unmap(m_pVertexBuffer, 0);
		
		draw(ren, tex);

		if ((point[0] > box3[0].x) & (point[1] > box3[3].y) & (point[0] < box3[3].x) & (point[1] < box3[0].y) & ((GetKeyState(VK_LBUTTON) & 0x80) != 0)) {
			madeChoice = true;
			selection = 3;
		}

		// Setup and draw decision box.
		box4[0].x = -0.5f;
		box4[0].y = -0.3f;
		box4[0].Xtex = 0.0f;
		box4[0].Ytex = 0.0f;

		box4[1].x = 0.5f;
		box4[1].y = -0.3f;
		box4[1].Xtex = 1.0f;
		box4[1].Ytex = 0.0f;

		box4[2].x = -0.5f;
		box4[2].y = -0.5f;
		box4[2].Xtex = 0.0f;
		box4[2].Ytex = 1.0f;

		box4[3].x = 0.5f;
		box4[3].y = -0.5f;
		box4[3].Xtex = 1.0f;
		box4[3].Ytex = 1.0f;

		if ((point[0] > box4[0].x) & (point[1] > box4[3].y) & (point[0] < box4[3].x) & (point[1] < box4[0].y))
		{
			box4[0].Ytex = 0.5f;
			box4[1].Ytex = 0.5f;
		}
		else
		{
			box4[2].Ytex = 0.5f;
			box4[3].Ytex = 0.5f;
		}

		ren.getDeviceContext()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, box4, sizeof(box4));
		ren.getDeviceContext()->Unmap(m_pVertexBuffer, 0);
		
		draw(ren, tex);

		if ((point[0] > box4[0].x) & (point[1] > box4[3].y) & (point[0] < box4[3].x) & (point[1] < box4[0].y) & ((GetKeyState(VK_LBUTTON) & 0x80) != 0)) {
			madeChoice = true;
			selection = 4;
		}
	}
}
char * Choice::setScene() {
	char * token;
	char * nextTok;

	if (madeChoice) {
		token = strtok_s(sceneBuffer, ", ", &nextTok);
		selection--;
		while (selection > 0) {
			token = strtok_s(NULL, ", ", &nextTok);
			selection--;
		}
	}
	else {
		// We've messed up. A bug has occured where the user has not selected a choice but the program reached here.
		exit(13);
	}

	return token;
}
