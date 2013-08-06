#include "Appearance.h"

NEWDEL_IMPL(Appearance)
Appearance::Appearance() {
	m_pTexture = NULL;
	m_pShader = NULL;
	PVRTMatrixIdentity(mUVMtx);
}

void Appearance::SetTexture(Texture * pTexture) {
	if (pTexture == NULL)
	{
		STOP;
	}

	m_pTexture = pTexture;
}


void Appearance::SetShader(Shader * pShader) {
	m_pShader = pShader;
}

void Appearance::Apply() {
	mCompositingMode.Apply();
	//mPolygonMode.Apply();
	
	//mMaterial.Apply();

	m_pShader->UseProgram();
	m_pTexture->Apply();
}
