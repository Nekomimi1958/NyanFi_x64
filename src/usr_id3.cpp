//----------------------------------------------------------------------//
// MP3 ID3�^�O���̏���												//
//																		//
//----------------------------------------------------------------------//
#include "usr_str.h"
#include "usr_wic.h"
#include "usr_file_ex.h"
#include "usr_shell.h"
#include "usr_id3.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
unsigned int get_SynchsafeInt(BYTE *p)
{
	return (unsigned int)(((p[0]&0x7f) << 21) + ((p[1]&0x7f) << 14) + ((p[2]&0x7f) << 7) + (p[3] & 0x7f));
}
//---------------------------------------------------------------------------
unsigned int get_32bitInt(BYTE *p)
{
	return (unsigned int)((p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3]);
}
//---------------------------------------------------------------------------
unsigned int get_24bitInt(BYTE *p)
{
	return (unsigned int)((p[0] << 16) + (p[1] << 8) + p[2]);
}

//---------------------------------------------------------------------------
UnicodeString get_str_from_buf(char *c, int size)
{
	std::unique_ptr<char[]> ts(new char[size + 1]);
	char *p = ts.get();
	::CopyMemory(p, c, size);
	p += size;
	*p = '\0';
	return UnicodeString(ts.get());
}

//---------------------------------------------------------------------------
//�o�[�W�������擾���A�擪�t���[���w�b�_��
//  �߂�l: 1=v1?  3=v2.3  4=v2.4
//---------------------------------------------------------------------------
bool ID3_getVer_toFrame(TFileStream *fs, unsigned int *last, unsigned int *majv)
{
	try {
		id3_hdr idhdr;
		fs->ReadBuffer(&idhdr, 10);
		if (idhdr.hdr_id[0]!='I' || idhdr.hdr_id[1]!='D' || idhdr.hdr_id[2]!='3') Abort();
		unsigned int size = get_SynchsafeInt(&idhdr.size[0]);
		*last = size + 10;
		*majv = idhdr.maj_ver;
		if (idhdr.maj_ver!=2 && idhdr.maj_ver!=3 && idhdr.maj_ver!=4) Abort();
		if (idhdr.maj_ver>=3 && (idhdr.flag & 0x40)) {
			//�g���w�b�_������Γǂݔ�΂�
			BYTE exsz[4];
			id3_ex_hdr exhdr;
			fs->ReadBuffer(&exsz[0], 4);
			int ex_size = (idhdr.maj_ver==4)? get_SynchsafeInt(&exsz[0]) : get_32bitInt(&exsz[0]);
			if (idhdr.maj_ver==4) ex_size-=4,
			fs->Seek(ex_size, soFromCurrent);
		}
		return true;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//MP3�t�@�C���� ID3 Tag v2/v1 �����擾
//---------------------------------------------------------------------------
bool ID3_GetInf(
	UnicodeString fnam,		//�t�@�C����
	TStringList *lst,		//���ʃ��X�g(����=���e �̌`���Ŋi�[)
	bool id_min)			//�ŏ����̏��� T??=�̌`���Ŏ擾
{
	bool ret = false;
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));

		unsigned int maj_ver = 0, last_p = 0;
		if (ID3_getVer_toFrame(fs.get(), &last_p, &maj_ver)) {
			//ID3v2
			lst->Add(get_PropTitle(_T("ID3�o�[�W����")).cat_sprintf(_T("2.%u"), maj_ver));

			//�t���[���w�b�_
			for (;;) {
				id3_frm_hdr   frmhdr;
				id3_frm_hdr_2 frmhdr2;
				UnicodeString id_str;
				if (maj_ver==2) {
					fs->ReadBuffer(&frmhdr2, 6);
					id_str = get_str_from_buf(frmhdr2.frame_id, 3);
				}
				else {
					fs->ReadBuffer(&frmhdr, 10);
					id_str = get_str_from_buf(frmhdr.frame_id, 4);
				}
				if (id_str.IsEmpty()) break;

				int size;
				switch (maj_ver) {
				case 2: size = get_24bitInt(&frmhdr2.size[0]);		break;
				case 3: size = get_32bitInt(&frmhdr.size[0]);		break;
				case 4: size = get_SynchsafeInt(&frmhdr.size[0]);	break;
				}

				if (id_str[1]=='T')  {
					//�e�L�X�g�G���R�[�f�B���O
					BYTE code = fsRead_byte(fs.get());
					size--;
					//�\�����ɕϊ�
					UnicodeString lbuf = id_str;
					if (id_min) {
						if		(contained_wd_i(_T("TPE1|TP1"), id_str)) lbuf = "TP1=";
						else if (contained_wd_i(_T("TALB|TAL"), id_str)) lbuf = "TAL=";
						else if (contained_wd_i(_T("TIT2|TT2"), id_str)) lbuf = "TT2=";
						else if (contained_wd_i(_T("TRCK|TRK"), id_str)) lbuf = "TRK=";
						else if (contained_wd_i(_T("TYER|TYE"), id_str)) lbuf = "TYE=";
						else										     lbuf = EmptyStr;
					}
					else {
						if		(contained_wd_i(_T("TALB|TAL"), id_str)) lbuf = "�A���o����";
						else if (contained_wd_i(_T("TCOM|TCM"), id_str)) lbuf = "��Ȏ�";
						else if (contained_wd_i(_T("TCON|TCO"), id_str)) lbuf = "�W������";
						else if (contained_wd_i(_T("TCOP|TCR"), id_str)) lbuf = "���쌠���";
						else if (contained_wd_i(_T("TDAT|TDA"), id_str)) lbuf = "���t";
						else if (contained_wd_i(_T("TENC|TEN"), id_str)) lbuf = "�G���R�[�h";
						else if (contained_wd_i(_T("TIT1|TT1"), id_str)) lbuf = "�O���[�v";
						else if (contained_wd_i(_T("TIT2|TT2"), id_str)) lbuf = "�^�C�g��";
						else if (contained_wd_i(_T("TIT3|TT3"), id_str)) lbuf = "�T�u�^�C�g��";
						else if (contained_wd_i(_T("TLEN|TLE"), id_str)) lbuf = "�Ȃ̒���";
						else if (contained_wd_i(_T("TORY|TOR|TDOR"), id_str))
														 	 	   		 lbuf = "���ȃ����[�X�N";
						else if (contained_wd_i(_T("TPE1|TP1"), id_str)) lbuf = "�A�[�e�B�X�g";
						else if (contained_wd_i(_T("TPE2|TP2"), id_str)) lbuf = "�o���h/���t";
						else if (contained_wd_i(_T("TPE3|TP3"), id_str)) lbuf = "�w����";
						else if (contained_wd_i(_T("TPOS|TPA"), id_str)) lbuf = "�f�B�X�N��";
						else if (contained_wd_i(_T("TPUB|TPB"), id_str)) lbuf = "�o�Ŏ�";
						else if (contained_wd_i(_T("TRCK|TRK"), id_str)) lbuf = "�g���b�N��";
						else if (contained_wd_i(_T("TRDA|TRD|TDRC"), id_str))
															 			 lbuf = "�^������";
						else if (contained_wd_i(_T("TYER|TYE"), id_str)) lbuf = "�����[�X�N";
						else											 lbuf = id_str;
						lbuf = get_PropTitle(lbuf);
					}

					//������
					std::unique_ptr<TMemoryStream> ms(new TMemoryStream());
					if (ms->CopyFrom(fs.get(), size)<size) Abort();
					ms->Seek(0, soFromBeginning);
					std::unique_ptr<TStringList> ts(new TStringList());
					switch (code) {
					case 3:	 ts->LoadFromStream(ms.get(), TEncoding::UTF8); break;
					default: ts->LoadFromStream(ms.get());
					}
					if (ts->Count>0 && !lbuf.IsEmpty()) lst->Add(lbuf + Trim(ts->Strings[0]));
				}
				else {
					fs->Seek(size, soFromCurrent);
				}
				if (fs->Position>=last_p) break;
			}

			ret = true;
		}
		else {
			//ID3v1
			char fbuf[128];
			fs->Seek(-128, soFromEnd);
			fs->ReadBuffer(fbuf, 128);
			std::unique_ptr<char[]> sbuf(new char[32]);
			char *sp = sbuf.get();
			UnicodeString tmp;
			char *s = &fbuf[0]; memcpy(sp, s, 3); sp[ 3] = '\0'; tmp = sp;
			if (USAME_TI(tmp, "TAG")) {
				add_PropLine(_T("ID3�o�[�W����"), "1", lst);
				s = &fbuf[ 3]; memcpy(sp, s, 30); sp[30] = '\0'; tmp = sp;
				lst->Add((id_min? UnicodeString("TT2=") : get_PropTitle(_T("�g���b�N��"))) + tmp);
				s = &fbuf[33]; memcpy(sp, s, 30); sp[30] = '\0'; tmp = sp;
				lst->Add((id_min? UnicodeString("TP1=") : get_PropTitle(_T("�A�[�e�B�X�g��"))) + tmp);
				s = &fbuf[63]; memcpy(sp, s, 30); sp[30] = '\0'; tmp = sp;
				lst->Add((id_min? UnicodeString("TAL=") : get_PropTitle(_T("�A���o����"))) + tmp);
				s = &fbuf[93]; memcpy(sp, s,  4); sp[ 4] = '\0'; tmp = sp;
				lst->Add((id_min? UnicodeString("TYE=") : get_PropTitle(_T("�����[�X�N"))) + tmp);
				s = &fbuf[97]; memcpy(sp, s, 30); sp[30] = '\0'; tmp = sp;
				if (!id_min) add_PropLine(_T("�R�����g"), tmp, lst);
				ret = true;
			}
		}
	}
	catch (...) {
		lst->Clear();
	}
	return ret;
}

//---------------------------------------------------------------------------
//MP3�t�@�C�����疄�ߍ��݉摜���r�b�g�}�b�v�Ŏ擾 or �t�@�C���ɏo��
//---------------------------------------------------------------------------
bool ID3_GetImage(
	UnicodeString fnam,			//�t�@�C����
	Graphics::TBitmap *bmpimg,	//�o�̓r�b�g�}�b�v (default = NULL)
	UnicodeString o_fnam)		//�o�̓t�@�C����	 (default = EmptyStr)
{
	bool ok = false;
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));

		//ID3v2�^�O�w�b�_
		unsigned int maj_ver = 0, last_p = 0;
		if (!ID3_getVer_toFrame(fs.get(), &last_p, &maj_ver)) Abort();

		//�t���[���w�b�_
		for (;;) {
			id3_frm_hdr   frmhdr;
			id3_frm_hdr_2 frmhdr2;
			UnicodeString id_str;
			if (maj_ver==2) {
				fs->ReadBuffer(&frmhdr2, 6);
				id_str = get_str_from_buf(frmhdr2.frame_id, 3);
			}
			else {
				fs->ReadBuffer(&frmhdr, 10);
				id_str = get_str_from_buf(frmhdr.frame_id, 4);
			}
			if (id_str.IsEmpty()) break;

			int size;
			switch (maj_ver) {
			case 2: size = get_24bitInt(&frmhdr2.size[0]);		break;
			case 3: size = get_32bitInt(&frmhdr.size[0]);		break;
			case 4: size = get_SynchsafeInt(&frmhdr.size[0]);	break;
			}

			if (contained_wd_i(_T("APIC|PIC"), id_str)) {
				//�e�L�X�g�G���R�[�f�B���O
				BYTE code = fsRead_byte(fs.get());
				size--;
				bool is_16 = (code==1 || code==2);

				std::unique_ptr<char[]> tbuf(new char[32]);
				//�摜�`��
				if (maj_ver==2) {
					fs->ReadBuffer(tbuf.get(), 3);
					tbuf[3] = '\0';
					size -= 3;
				}
				//MIMI�^�C�v
				else {
					char *pb = tbuf.get();
					for (int i=0; i<32; i++,pb++) {
						fs->ReadBuffer(pb, 1);
						size--;
						if (*pb=='\0') break;
					}
				}
				UnicodeString img_typ = tbuf.get();

				//�摜�^�C�v
				BYTE pic_typ;
				fs->ReadBuffer(&pic_typ, 1);
				size--;
				//Description �͓ǂݔ�΂�
				int nul_cnt = 0;
				for (;;) {
					char d = fsRead_byte(fs.get());
					size--;
					if (d!='\0') continue;
					nul_cnt++;
					if (!is_16 && nul_cnt==1) break;
					if ( is_16 && nul_cnt==2) break;
				}
				//�C���[�W
				std::unique_ptr<TMemoryStream> ms(new TMemoryStream());
				if (ms->CopyFrom(fs.get(), size)<size) Abort();
				ms->Seek(0, soFromBeginning);

				if (contained_wd_i(_T("image/jpeg|jpg"), img_typ)) {
					if (bmpimg) {
						if (!WIC_get_from_memory(ms.get(), bmpimg, GUID_ContainerFormatJpeg)) Abort();
						ok = !bmpimg->Empty;
					}
					else if (!o_fnam.IsEmpty()) {
						o_fnam = ChangeFileExt(o_fnam, ".jpg");
						ms->SaveToFile(o_fnam);
						ok = true;
					}
				}

				else if (contained_wd_i(_T("image/png|png"), img_typ)) {
					if (bmpimg) {
						if (!WIC_get_from_memory(ms.get(), bmpimg, GUID_ContainerFormatPng)) Abort();
						ok = !bmpimg->Empty;
					}
					else if (!o_fnam.IsEmpty()) {
						o_fnam = ChangeFileExt(o_fnam, ".png");
						ms->SaveToFile(o_fnam);
						ok = true;
					}
				}
				break;
			}
			else {
				fs->Seek(size, soFromCurrent);
			}
			if (fs->Position>=last_p) break;
		}
	}
	catch (...) {
		ok = false;
	}

	return ok;
}
//---------------------------------------------------------------------------
