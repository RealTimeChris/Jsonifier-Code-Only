/*
	MIT License

	Copyright (c) 2023 RealTimeChris

	Permission is hereby granted, free of charge, to any person obtaining a copy of this
	software and associated documentation files (the "Software"), to deal in the Software
	without restriction, including without limitation the rights to use, copy, modify, merge,
	publish, distribute, sublicense, and/or sell copies of the Software, and to permit
	persons to whom the Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or
	substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
	INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
	PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.
*/
/// Note: Most of the code in this header was sampled from Glaze library: https://github.com/stephenberry/glaze
/// https://github.com/RealTimeChris/jsonifier
/// Nov 13, 2023
#pragma once

#include <jsonifier/Allocator.hpp>
#include <jsonifier/Iterator.hpp>

#include <functional>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <vector>
#include <array>

#if defined(_M_X64) || defined(_M_ARM64)
	#include <intrin.h>
#endif

namespace jsonifier_internal {

#if defined(__SIZEOF_INT128__)
	JSONIFIER_INLINE uint64_t mulhi64(uint64_t a, uint64_t b) {
	#if JSONIFIER_GNUCXX
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wpedantic"
		unsigned __int128 prod = a * static_cast<unsigned __int128>(b);
		#pragma GCC diagnostic pop
	#else
		unsigned __int128 prod = a * static_cast<unsigned __int128>(b);
	#endif
		return prod >> 64;
	}
#elif defined(_M_X64) || defined(_M_ARM64)
	#define mulhi64 __umulh
#else
	JSONIFIER_INLINE uint64_t mulhi64(uint64_t a, uint64_t b) noexcept {
		uint64_t aLo	  = ( uint32_t )a;
		uint64_t aHi	  = a >> 32;
		uint64_t bLo	  = ( uint32_t )b;
		uint64_t bHi	  = b >> 32;
		uint64_t axbHi	  = aHi * bHi;
		uint64_t axbMid	  = aHi * bLo;
		uint64_t bxaMid	  = bHi * aLo;
		uint64_t axbLo	  = aLo * bLo;
		uint64_t carryBit = (( uint64_t )( uint32_t )axbMid + ( uint64_t )( uint32_t )bxaMid + (axbLo >> 32)) >> 32;
		uint64_t multhi	  = axbHi + (axbMid >> 32) + (bxaMid >> 32) + carryBit;
		return multhi;
	}
#endif

	constexpr auto pow10SigTableMinExp	 = -343;
	constexpr auto pow10SigTableMaxExp	 = 324;
	constexpr auto pow10SigTableMinExact = 0;
	constexpr auto pow10SigTableMaxExact = 27;

	constexpr std::array<uint64_t, 668> pow10SigTable{ 0xBF29DCABA82FDEAE, 0xEEF453D6923BD65A, 0x9558B4661B6565F8, 0xBAAEE17FA23EBF76, 0xE95A99DF8ACE6F53, 0x91D8A02BB6C10594,
		0xB64EC836A47146F9, 0xE3E27A444D8D98B7, 0x8E6D8C6AB0787F72, 0xB208EF855C969F4F, 0xDE8B2B66B3BC4723, 0x8B16FB203055AC76, 0xADDCB9E83C6B1793, 0xD953E8624B85DD78,
		0x87D4713D6F33AA6B, 0xA9C98D8CCB009506, 0xD43BF0EFFDC0BA48, 0x84A57695FE98746D, 0xA5CED43B7E3E9188, 0xCF42894A5DCE35EA, 0x818995CE7AA0E1B2, 0xA1EBFB4219491A1F,
		0xCA66FA129F9B60A6, 0xFD00B897478238D0, 0x9E20735E8CB16382, 0xC5A890362FDDBC62, 0xF712B443BBD52B7B, 0x9A6BB0AA55653B2D, 0xC1069CD4EABE89F8, 0xF148440A256E2C76,
		0x96CD2A865764DBCA, 0xBC807527ED3E12BC, 0xEBA09271E88D976B, 0x93445B8731587EA3, 0xB8157268FDAE9E4C, 0xE61ACF033D1A45DF, 0x8FD0C16206306BAB, 0xB3C4F1BA87BC8696,
		0xE0B62E2929ABA83C, 0x8C71DCD9BA0B4925, 0xAF8E5410288E1B6F, 0xDB71E91432B1A24A, 0x892731AC9FAF056E, 0xAB70FE17C79AC6CA, 0xD64D3D9DB981787D, 0x85F0468293F0EB4E,
		0xA76C582338ED2621, 0xD1476E2C07286FAA, 0x82CCA4DB847945CA, 0xA37FCE126597973C, 0xCC5FC196FEFD7D0C, 0xFF77B1FCBEBCDC4F, 0x9FAACF3DF73609B1, 0xC795830D75038C1D,
		0xF97AE3D0D2446F25, 0x9BECCE62836AC577, 0xC2E801FB244576D5, 0xF3A20279ED56D48A, 0x9845418C345644D6, 0xBE5691EF416BD60C, 0xEDEC366B11C6CB8F, 0x94B3A202EB1C3F39,
		0xB9E08A83A5E34F07, 0xE858AD248F5C22C9, 0x91376C36D99995BE, 0xB58547448FFFFB2D, 0xE2E69915B3FFF9F9, 0x8DD01FAD907FFC3B, 0xB1442798F49FFB4A, 0xDD95317F31C7FA1D,
		0x8A7D3EEF7F1CFC52, 0xAD1C8EAB5EE43B66, 0xD863B256369D4A40, 0x873E4F75E2224E68, 0xA90DE3535AAAE202, 0xD3515C2831559A83, 0x8412D9991ED58091, 0xA5178FFF668AE0B6,
		0xCE5D73FF402D98E3, 0x80FA687F881C7F8E, 0xA139029F6A239F72, 0xC987434744AC874E, 0xFBE9141915D7A922, 0x9D71AC8FADA6C9B5, 0xC4CE17B399107C22, 0xF6019DA07F549B2B,
		0x99C102844F94E0FB, 0xC0314325637A1939, 0xF03D93EEBC589F88, 0x96267C7535B763B5, 0xBBB01B9283253CA2, 0xEA9C227723EE8BCB, 0x92A1958A7675175F, 0xB749FAED14125D36,
		0xE51C79A85916F484, 0x8F31CC0937AE58D2, 0xB2FE3F0B8599EF07, 0xDFBDCECE67006AC9, 0x8BD6A141006042BD, 0xAECC49914078536D, 0xDA7F5BF590966848, 0x888F99797A5E012D,
		0xAAB37FD7D8F58178, 0xD5605FCDCF32E1D6, 0x855C3BE0A17FCD26, 0xA6B34AD8C9DFC06F, 0xD0601D8EFC57B08B, 0x823C12795DB6CE57, 0xA2CB1717B52481ED, 0xCB7DDCDDA26DA268,
		0xFE5D54150B090B02, 0x9EFA548D26E5A6E1, 0xC6B8E9B0709F109A, 0xF867241C8CC6D4C0, 0x9B407691D7FC44F8, 0xC21094364DFB5636, 0xF294B943E17A2BC4, 0x979CF3CA6CEC5B5A,
		0xBD8430BD08277231, 0xECE53CEC4A314EBD, 0x940F4613AE5ED136, 0xB913179899F68584, 0xE757DD7EC07426E5, 0x9096EA6F3848984F, 0xB4BCA50B065ABE63, 0xE1EBCE4DC7F16DFB,
		0x8D3360F09CF6E4BD, 0xB080392CC4349DEC, 0xDCA04777F541C567, 0x89E42CAAF9491B60, 0xAC5D37D5B79B6239, 0xD77485CB25823AC7, 0x86A8D39EF77164BC, 0xA8530886B54DBDEB,
		0xD267CAA862A12D66, 0x8380DEA93DA4BC60, 0xA46116538D0DEB78, 0xCD795BE870516656, 0x806BD9714632DFF6, 0xA086CFCD97BF97F3, 0xC8A883C0FDAF7DF0, 0xFAD2A4B13D1B5D6C,
		0x9CC3A6EEC6311A63, 0xC3F490AA77BD60FC, 0xF4F1B4D515ACB93B, 0x991711052D8BF3C5, 0xBF5CD54678EEF0B6, 0xEF340A98172AACE4, 0x9580869F0E7AAC0E, 0xBAE0A846D2195712,
		0xE998D258869FACD7, 0x91FF83775423CC06, 0xB67F6455292CBF08, 0xE41F3D6A7377EECA, 0x8E938662882AF53E, 0xB23867FB2A35B28D, 0xDEC681F9F4C31F31, 0x8B3C113C38F9F37E,
		0xAE0B158B4738705E, 0xD98DDAEE19068C76, 0x87F8A8D4CFA417C9, 0xA9F6D30A038D1DBC, 0xD47487CC8470652B, 0x84C8D4DFD2C63F3B, 0xA5FB0A17C777CF09, 0xCF79CC9DB955C2CC,
		0x81AC1FE293D599BF, 0xA21727DB38CB002F, 0xCA9CF1D206FDC03B, 0xFD442E4688BD304A, 0x9E4A9CEC15763E2E, 0xC5DD44271AD3CDBA, 0xF7549530E188C128, 0x9A94DD3E8CF578B9,
		0xC13A148E3032D6E7, 0xF18899B1BC3F8CA1, 0x96F5600F15A7B7E5, 0xBCB2B812DB11A5DE, 0xEBDF661791D60F56, 0x936B9FCEBB25C995, 0xB84687C269EF3BFB, 0xE65829B3046B0AFA,
		0x8FF71A0FE2C2E6DC, 0xB3F4E093DB73A093, 0xE0F218B8D25088B8, 0x8C974F7383725573, 0xAFBD2350644EEACF, 0xDBAC6C247D62A583, 0x894BC396CE5DA772, 0xAB9EB47C81F5114F,
		0xD686619BA27255A2, 0x8613FD0145877585, 0xA798FC4196E952E7, 0xD17F3B51FCA3A7A0, 0x82EF85133DE648C4, 0xA3AB66580D5FDAF5, 0xCC963FEE10B7D1B3, 0xFFBBCFE994E5C61F,
		0x9FD561F1FD0F9BD3, 0xC7CABA6E7C5382C8, 0xF9BD690A1B68637B, 0x9C1661A651213E2D, 0xC31BFA0FE5698DB8, 0xF3E2F893DEC3F126, 0x986DDB5C6B3A76B7, 0xBE89523386091465,
		0xEE2BA6C0678B597F, 0x94DB483840B717EF, 0xBA121A4650E4DDEB, 0xE896A0D7E51E1566, 0x915E2486EF32CD60, 0xB5B5ADA8AAFF80B8, 0xE3231912D5BF60E6, 0x8DF5EFABC5979C8F,
		0xB1736B96B6FD83B3, 0xDDD0467C64BCE4A0, 0x8AA22C0DBEF60EE4, 0xAD4AB7112EB3929D, 0xD89D64D57A607744, 0x87625F056C7C4A8B, 0xA93AF6C6C79B5D2D, 0xD389B47879823479,
		0x843610CB4BF160CB, 0xA54394FE1EEDB8FE, 0xCE947A3DA6A9273E, 0x811CCC668829B887, 0xA163FF802A3426A8, 0xC9BCFF6034C13052, 0xFC2C3F3841F17C67, 0x9D9BA7832936EDC0,
		0xC5029163F384A931, 0xF64335BCF065D37D, 0x99EA0196163FA42E, 0xC06481FB9BCF8D39, 0xF07DA27A82C37088, 0x964E858C91BA2655, 0xBBE226EFB628AFEA, 0xEADAB0ABA3B2DBE5,
		0x92C8AE6B464FC96F, 0xB77ADA0617E3BBCB, 0xE55990879DDCAABD, 0x8F57FA54C2A9EAB6, 0xB32DF8E9F3546564, 0xDFF9772470297EBD, 0x8BFBEA76C619EF36, 0xAEFAE51477A06B03,
		0xDAB99E59958885C4, 0x88B402F7FD75539B, 0xAAE103B5FCD2A881, 0xD59944A37C0752A2, 0x857FCAE62D8493A5, 0xA6DFBD9FB8E5B88E, 0xD097AD07A71F26B2, 0x825ECC24C873782F,
		0xA2F67F2DFA90563B, 0xCBB41EF979346BCA, 0xFEA126B7D78186BC, 0x9F24B832E6B0F436, 0xC6EDE63FA05D3143, 0xF8A95FCF88747D94, 0x9B69DBE1B548CE7C, 0xC24452DA229B021B,
		0xF2D56790AB41C2A2, 0x97C560BA6B0919A5, 0xBDB6B8E905CB600F, 0xED246723473E3813, 0x9436C0760C86E30B, 0xB94470938FA89BCE, 0xE7958CB87392C2C2, 0x90BD77F3483BB9B9,
		0xB4ECD5F01A4AA828, 0xE2280B6C20DD5232, 0x8D590723948A535F, 0xB0AF48EC79ACE837, 0xDCDB1B2798182244, 0x8A08F0F8BF0F156B, 0xAC8B2D36EED2DAC5, 0xD7ADF884AA879177,
		0x86CCBB52EA94BAEA, 0xA87FEA27A539E9A5, 0xD29FE4B18E88640E, 0x83A3EEEEF9153E89, 0xA48CEAAAB75A8E2B, 0xCDB02555653131B6, 0x808E17555F3EBF11, 0xA0B19D2AB70E6ED6,
		0xC8DE047564D20A8B, 0xFB158592BE068D2E, 0x9CED737BB6C4183D, 0xC428D05AA4751E4C, 0xF53304714D9265DF, 0x993FE2C6D07B7FAB, 0xBF8FDB78849A5F96, 0xEF73D256A5C0F77C,
		0x95A8637627989AAD, 0xBB127C53B17EC159, 0xE9D71B689DDE71AF, 0x9226712162AB070D, 0xB6B00D69BB55C8D1, 0xE45C10C42A2B3B05, 0x8EB98A7A9A5B04E3, 0xB267ED1940F1C61C,
		0xDF01E85F912E37A3, 0x8B61313BBABCE2C6, 0xAE397D8AA96C1B77, 0xD9C7DCED53C72255, 0x881CEA14545C7575, 0xAA242499697392D2, 0xD4AD2DBFC3D07787, 0x84EC3C97DA624AB4,
		0xA6274BBDD0FADD61, 0xCFB11EAD453994BA, 0x81CEB32C4B43FCF4, 0xA2425FF75E14FC31, 0xCAD2F7F5359A3B3E, 0xFD87B5F28300CA0D, 0x9E74D1B791E07E48, 0xC612062576589DDA,
		0xF79687AED3EEC551, 0x9ABE14CD44753B52, 0xC16D9A0095928A27, 0xF1C90080BAF72CB1, 0x971DA05074DA7BEE, 0xBCE5086492111AEA, 0xEC1E4A7DB69561A5, 0x9392EE8E921D5D07,
		0xB877AA3236A4B449, 0xE69594BEC44DE15B, 0x901D7CF73AB0ACD9, 0xB424DC35095CD80F, 0xE12E13424BB40E13, 0x8CBCCC096F5088CB, 0xAFEBFF0BCB24AAFE, 0xDBE6FECEBDEDD5BE,
		0x89705F4136B4A597, 0xABCC77118461CEFC, 0xD6BF94D5E57A42BC, 0x8637BD05AF6C69B5, 0xA7C5AC471B478423, 0xD1B71758E219652B, 0x83126E978D4FDF3B, 0xA3D70A3D70A3D70A,
		0xCCCCCCCCCCCCCCCC, 0x8000000000000000, 0xA000000000000000, 0xC800000000000000, 0xFA00000000000000, 0x9C40000000000000, 0xC350000000000000, 0xF424000000000000,
		0x9896800000000000, 0xBEBC200000000000, 0xEE6B280000000000, 0x9502F90000000000, 0xBA43B74000000000, 0xE8D4A51000000000, 0x9184E72A00000000, 0xB5E620F480000000,
		0xE35FA931A0000000, 0x8E1BC9BF04000000, 0xB1A2BC2EC5000000, 0xDE0B6B3A76400000, 0x8AC7230489E80000, 0xAD78EBC5AC620000, 0xD8D726B7177A8000, 0x878678326EAC9000,
		0xA968163F0A57B400, 0xD3C21BCECCEDA100, 0x84595161401484A0, 0xA56FA5B99019A5C8, 0xCECB8F27F4200F3A, 0x813F3978F8940984, 0xA18F07D736B90BE5, 0xC9F2C9CD04674EDE,
		0xFC6F7C4045812296, 0x9DC5ADA82B70B59D, 0xC5371912364CE305, 0xF684DF56C3E01BC6, 0x9A130B963A6C115C, 0xC097CE7BC90715B3, 0xF0BDC21ABB48DB20, 0x96769950B50D88F4,
		0xBC143FA4E250EB31, 0xEB194F8E1AE525FD, 0x92EFD1B8D0CF37BE, 0xB7ABC627050305AD, 0xE596B7B0C643C719, 0x8F7E32CE7BEA5C6F, 0xB35DBF821AE4F38B, 0xE0352F62A19E306E,
		0x8C213D9DA502DE45, 0xAF298D050E4395D6, 0xDAF3F04651D47B4C, 0x88D8762BF324CD0F, 0xAB0E93B6EFEE0053, 0xD5D238A4ABE98068, 0x85A36366EB71F041, 0xA70C3C40A64E6C51,
		0xD0CF4B50CFE20765, 0x82818F1281ED449F, 0xA321F2D7226895C7, 0xCBEA6F8CEB02BB39, 0xFEE50B7025C36A08, 0x9F4F2726179A2245, 0xC722F0EF9D80AAD6, 0xF8EBAD2B84E0D58B,
		0x9B934C3B330C8577, 0xC2781F49FFCFA6D5, 0xF316271C7FC3908A, 0x97EDD871CFDA3A56, 0xBDE94E8E43D0C8EC, 0xED63A231D4C4FB27, 0x945E455F24FB1CF8, 0xB975D6B6EE39E436,
		0xE7D34C64A9C85D44, 0x90E40FBEEA1D3A4A, 0xB51D13AEA4A488DD, 0xE264589A4DCDAB14, 0x8D7EB76070A08AEC, 0xB0DE65388CC8ADA8, 0xDD15FE86AFFAD912, 0x8A2DBF142DFCC7AB,
		0xACB92ED9397BF996, 0xD7E77A8F87DAF7FB, 0x86F0AC99B4E8DAFD, 0xA8ACD7C0222311BC, 0xD2D80DB02AABD62B, 0x83C7088E1AAB65DB, 0xA4B8CAB1A1563F52, 0xCDE6FD5E09ABCF26,
		0x80B05E5AC60B6178, 0xA0DC75F1778E39D6, 0xC913936DD571C84C, 0xFB5878494ACE3A5F, 0x9D174B2DCEC0E47B, 0xC45D1DF942711D9A, 0xF5746577930D6500, 0x9968BF6ABBE85F20,
		0xBFC2EF456AE276E8, 0xEFB3AB16C59B14A2, 0x95D04AEE3B80ECE5, 0xBB445DA9CA61281F, 0xEA1575143CF97226, 0x924D692CA61BE758, 0xB6E0C377CFA2E12E, 0xE498F455C38B997A,
		0x8EDF98B59A373FEC, 0xB2977EE300C50FE7, 0xDF3D5E9BC0F653E1, 0x8B865B215899F46C, 0xAE67F1E9AEC07187, 0xDA01EE641A708DE9, 0x884134FE908658B2, 0xAA51823E34A7EEDE,
		0xD4E5E2CDC1D1EA96, 0x850FADC09923329E, 0xA6539930BF6BFF45, 0xCFE87F7CEF46FF16, 0x81F14FAE158C5F6E, 0xA26DA3999AEF7749, 0xCB090C8001AB551C, 0xFDCB4FA002162A63,
		0x9E9F11C4014DDA7E, 0xC646D63501A1511D, 0xF7D88BC24209A565, 0x9AE757596946075F, 0xC1A12D2FC3978937, 0xF209787BB47D6B84, 0x9745EB4D50CE6332, 0xBD176620A501FBFF,
		0xEC5D3FA8CE427AFF, 0x93BA47C980E98CDF, 0xB8A8D9BBE123F017, 0xE6D3102AD96CEC1D, 0x9043EA1AC7E41392, 0xB454E4A179DD1877, 0xE16A1DC9D8545E94, 0x8CE2529E2734BB1D,
		0xB01AE745B101E9E4, 0xDC21A1171D42645D, 0x899504AE72497EBA, 0xABFA45DA0EDBDE69, 0xD6F8D7509292D603, 0x865B86925B9BC5C2, 0xA7F26836F282B732, 0xD1EF0244AF2364FF,
		0x8335616AED761F1F, 0xA402B9C5A8D3A6E7, 0xCD036837130890A1, 0x802221226BE55A64, 0xA02AA96B06DEB0FD, 0xC83553C5C8965D3D, 0xFA42A8B73ABBF48C, 0x9C69A97284B578D7,
		0xC38413CF25E2D70D, 0xF46518C2EF5B8CD1, 0x98BF2F79D5993802, 0xBEEEFB584AFF8603, 0xEEAABA2E5DBF6784, 0x952AB45CFA97A0B2, 0xBA756174393D88DF, 0xE912B9D1478CEB17,
		0x91ABB422CCB812EE, 0xB616A12B7FE617AA, 0xE39C49765FDF9D94, 0x8E41ADE9FBEBC27D, 0xB1D219647AE6B31C, 0xDE469FBD99A05FE3, 0x8AEC23D680043BEE, 0xADA72CCC20054AE9,
		0xD910F7FF28069DA4, 0x87AA9AFF79042286, 0xA99541BF57452B28, 0xD3FA922F2D1675F2, 0x847C9B5D7C2E09B7, 0xA59BC234DB398C25, 0xCF02B2C21207EF2E, 0x8161AFB94B44F57D,
		0xA1BA1BA79E1632DC, 0xCA28A291859BBF93, 0xFCB2CB35E702AF78, 0x9DEFBF01B061ADAB, 0xC56BAEC21C7A1916, 0xF6C69A72A3989F5B, 0x9A3C2087A63F6399, 0xC0CB28A98FCF3C7F,
		0xF0FDF2D3F3C30B9F, 0x969EB7C47859E743, 0xBC4665B596706114, 0xEB57FF22FC0C7959, 0x9316FF75DD87CBD8, 0xB7DCBF5354E9BECE, 0xE5D3EF282A242E81, 0x8FA475791A569D10,
		0xB38D92D760EC4455, 0xE070F78D3927556A, 0x8C469AB843B89562, 0xAF58416654A6BABB, 0xDB2E51BFE9D0696A, 0x88FCF317F22241E2, 0xAB3C2FDDEEAAD25A, 0xD60B3BD56A5586F1,
		0x85C7056562757456, 0xA738C6BEBB12D16C, 0xD106F86E69D785C7, 0x82A45B450226B39C, 0xA34D721642B06084, 0xCC20CE9BD35C78A5, 0xFF290242C83396CE, 0x9F79A169BD203E41,
		0xC75809C42C684DD1, 0xF92E0C3537826145, 0x9BBCC7A142B17CCB, 0xC2ABF989935DDBFE, 0xF356F7EBF83552FE, 0x98165AF37B2153DE, 0xBE1BF1B059E9A8D6, 0xEDA2EE1C7064130C,
		0x9485D4D1C63E8BE7, 0xB9A74A0637CE2EE1, 0xE8111C87C5C1BA99, 0x910AB1D4DB9914A0, 0xB54D5E4A127F59C8, 0xE2A0B5DC971F303A, 0x8DA471A9DE737E24, 0xB10D8E1456105DAD,
		0xDD50F1996B947518, 0x8A5296FFE33CC92F, 0xACE73CBFDC0BFB7B, 0xD8210BEFD30EFA5A, 0x8714A775E3E95C78, 0xA8D9D1535CE3B396, 0xD31045A8341CA07C, 0x83EA2B892091E44D,
		0xA4E4B66B68B65D60, 0xCE1DE40642E3F4B9, 0x80D2AE83E9CE78F3, 0xA1075A24E4421730, 0xC94930AE1D529CFC, 0xFB9B7CD9A4A7443C, 0x9D412E0806E88AA5, 0xC491798A08A2AD4E,
		0xF5B5D7EC8ACB58A2, 0x9991A6F3D6BF1765, 0xBFF610B0CC6EDD3F, 0xEFF394DCFF8A948E, 0x95F83D0A1FB69CD9, 0xBB764C4CA7A4440F, 0xEA53DF5FD18D5513, 0x92746B9BE2F8552C,
		0xB7118682DBB66A77, 0xE4D5E82392A40515, 0x8F05B1163BA6832D, 0xB2C71D5BCA9023F8, 0xDF78E4B2BD342CF6, 0x8BAB8EEFB6409C1A, 0xAE9672ABA3D0C320, 0xDA3C0F568CC4F3E8,
		0x8865899617FB1871, 0xAA7EEBFB9DF9DE8D, 0xD51EA6FA85785631, 0x8533285C936B35DE, 0xA67FF273B8460356, 0xD01FEF10A657842C, 0x8213F56A67F6B29B, 0xA298F2C501F45F42,
		0xCB3F2F7642717713, 0xFE0EFB53D30DD4D7, 0x9EC95D1463E8A506, 0xC67BB4597CE2CE48, 0xF81AA16FDC1B81DA, 0x9B10A4E5E9913128, 0xC1D4CE1F63F57D72, 0xF24A01A73CF2DCCF,
		0x976E41088617CA01, 0xBD49D14AA79DBC82, 0xEC9C459D51852BA2, 0x93E1AB8252F33B45, 0xB8DA1662E7B00A17, 0xE7109BFBA19C0C9D, 0x906A617D450187E2, 0xB484F9DC9641E9DA,
		0xE1A63853BBD26451, 0x8D07E33455637EB2, 0xB049DC016ABC5E5F, 0xDC5C5301C56B75F7, 0x89B9B3E11B6329BA, 0xAC2820D9623BF429, 0xD732290FBACAF133, 0x867F59A9D4BED6C0,
		0xA81F301449EE8C70, 0xD226FC195C6A2F8C, 0x83585D8FD9C25DB7, 0xA42E74F3D032F525, 0xCD3A1230C43FB26F, 0x80444B5E7AA7CF85, 0xA0555E361951C366, 0xC86AB5C39FA63440,
		0xFA856334878FC150, 0x9C935E00D4B9D8D2, 0xC3B8358109E84F07, 0xF4A642E14C6262C8, 0x98E7E9CCCFBD7DBD, 0xBF21E44003ACDD2C, 0xEEEA5D5004981478, 0x95527A5202DF0CCB,
		0xBAA718E68396CFFD, 0xE950DF20247C83FD, 0x91D28B7416CDD27E, 0xB6472E511C81471D, 0xE3D8F9E563A198E5, 0x8E679C2F5E44FF8F, 0xB201833B35D63F73, 0xDE81E40A034BCF4F,
		0x8B112E86420F6191, 0xADD57A27D29339F6, 0xD94AD8B1C7380874, 0x87CEC76F1C830548, 0xA9C2794AE3A3C69A, 0xD433179D9C8CB841, 0x849FEEC281D7F328, 0xA5C7EA73224DEFF3,
		0xCF39E50FEAE16BEF, 0x81842F29F2CCE375, 0xA1E53AF46F801C53, 0xCA5E89B18B602368, 0xFCF62C1DEE382C42, 0x9E19DB92B4E31BA9 };

	JSONIFIER_INLINE uint64_t sig2FromExp10(int32_t exp10) noexcept {
		return pow10SigTable[static_cast<uint64_t>(exp10) - static_cast<uint64_t>(pow10SigTableMinExp)];
	}

	JSONIFIER_INLINE int32_t exp2FromExp10(int32_t exp10) noexcept {
		return (((exp10 * 217706 - 4128768) >> 16) + 126);
	}

	constexpr uint8_t digiTypeZero	  = 1 << 0;
	constexpr uint8_t digiTypeNonZero = 1 << 1;
	constexpr uint8_t digiTypeDot	  = 1 << 4;
	constexpr uint8_t digiTypeExp	  = 1 << 5;

	template<typename uint8_t> constexpr std::array<uint8_t, 256> digiTable{ 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
		0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
		0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x04u, 0x00u, '\b', 0x10u, 0x00u, 0x01u, 0x02u, 0x02u, 0x02u, 0x02u, 0x02u, 0x02u, 0x02u, 0x02u, 0x02u, 0x00u, 0x00u, 0x00u, 0x00u,
		0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x20u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
		0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x20u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
		0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u };

	template<typename uint8_t> JSONIFIER_INLINE bool digiIsType(uint8_t d, uint8_t type) noexcept {
		return (digiTable<uint8_t>[static_cast<uint64_t>(d)] & type) != 0;
	}

	template<typename uint8_t> JSONIFIER_INLINE bool digiIsFp(uint8_t d) noexcept {
		return digiIsType(d, uint8_t(digiTypeDot | digiTypeExp));
	}

	template<typename uint8_t> JSONIFIER_INLINE bool digiIsDigitOrFp(uint8_t d) noexcept {
		return digiIsType(d, uint8_t(digiTypeZero | digiTypeNonZero | digiTypeDot | digiTypeExp));
	}

	template<jsonifier::concepts::uint16_type return_type> constexpr return_type hasZero(const return_type value) noexcept {
		return (((value - 0x0101) & ~value) & 0x8080);
	}

	template<jsonifier::concepts::uint32_type return_type> constexpr return_type hasZero(const return_type value) noexcept {
		return (((value - 0x01010101) & ~value) & 0x80808080);
	}

	template<jsonifier::concepts::uint64_type return_type> constexpr return_type hasZero(const return_type value) noexcept {
		return (((value - 0x0101010101010101) & ~value) & 0x8080808080808080);
	}

	template<auto valueNew, typename return_type> constexpr return_type hasValue(return_type value) noexcept {
		constexpr return_type newBytes{ repeatByte<valueNew, return_type>() };
		return hasZero(value ^ newBytes);
	}

	constexpr auto zero = static_cast<uint8_t>('0');

	constexpr std::array<uint64_t, 256> numberSubTable{ []() {
		std::array<uint64_t, 256> returnValues{};
		for (uint64_t x = 0; x < 256; ++x) {
			returnValues[x] = static_cast<uint64_t>(x - zero);
		}
		return returnValues;
	}() };

	constexpr std::array<bool, 256> digitTableBool{ []() {
		std::array<bool, 256> returnValues{};
		returnValues[0x30u] = true;
		returnValues[0x31u] = true;
		returnValues[0x32u] = true;
		returnValues[0x33u] = true;
		returnValues[0x34u] = true;
		returnValues[0x35u] = true;
		returnValues[0x36u] = true;
		returnValues[0x37u] = true;
		returnValues[0x38u] = true;
		returnValues[0x39u] = true;
		return returnValues;
	}() };

	constexpr std::array<uint64_t, 20> powersOfTenInt{ 1ull, 10ull, 100ull, 1000ull, 10000ull, 100000ull, 1000000ull, 10000000ull, 100000000ull, 1000000000ull, 10000000000ull,
		100000000000ull, 1000000000000ull, 10000000000000ull, 100000000000000ull, 1000000000000000ull, 10000000000000000ull, 100000000000000000ull, 1000000000000000000ull,
		10000000000000000000ull };

	constexpr std::array<double, 23> powersOfTenFloat = { 1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10, 1e11, 1e12, 1e13, 1e14, 1e15, 1e16, 1e17, 1e18, 1e19, 1e20, 1e21,
		1e22 };

	consteval uint32_t ceillog2(uint32_t x) {
		return x < 2 ? x : 1 + ceillog2(x >> 1);
	}

	struct big_int_t final {
		std::vector<uint32_t> data = {};

		big_int_t(uint64_t num) noexcept {
			uint32_t lowerWord = uint32_t(num);
			uint32_t upperWord = uint32_t(num >> 32);
			if (upperWord > 0) {
				data = { lowerWord, upperWord };
			} else {
				data = { lowerWord };
			}
		}

		void mulU32(uint32_t num) noexcept {
			uint32_t carry = 0;
			for (uint64_t i = 0; i < data.size(); i++) {
				uint64_t res	   = uint64_t(data[i]) * uint64_t(num) + uint64_t(carry);
				uint32_t lowerWord = uint32_t(res);
				uint32_t upperWord = uint32_t(res >> 32);
				data[i]			   = lowerWord;
				carry			   = upperWord;
			}
			if (carry != 0) {
				data.emplace_back(carry);
			}
		}

		void mulPow10(uint32_t pow10) noexcept {
			for (; pow10 >= 9; pow10 -= 9) {
				mulU32(static_cast<uint32_t>(powersOfTenInt[9]));
			}
			if (pow10) {
				mulU32(static_cast<uint32_t>(powersOfTenInt[pow10]));
			}
		}

		void mulPow2(uint32_t exp) noexcept {
			uint32_t shft = exp % 32;
			uint32_t move = exp / 32;
			uint32_t idx  = static_cast<uint32_t>(data.size()) - 1;
			if (shft == 0) {
				data.resize(data.size() + move);
				for (; idx > 0; idx--) {
					data[static_cast<uint64_t>(idx) + static_cast<uint64_t>(move) - 1ull] = data[idx - 1ull];
				}
				while (move)
					data[--move] = 0;
			} else {
				data.resize(data.size() + move + 1);
				++idx;
				for (; idx > 0; idx--) {
					uint32_t num = data[idx] << shft;
					num |= data[idx - 1ull] >> (32 - shft);
					data[static_cast<uint64_t>(idx) + static_cast<uint64_t>(move)] = num;
				}
				data[move] = data[0] << shft;
				if (data.back() == 0)
					data.pop_back();
				while (move)
					data[--move] = 0;
			}
		}

		auto operator<=>(const big_int_t& rhs) const noexcept {
			if (data.size() < rhs.data.size())
				return -1;
			if (data.size() > rhs.data.size())
				return 1;
			for (auto i = data.size() - 1; i > 0; --i) {
				;
				if (data[i] < rhs.data[i])
					return -1;
				if (data[i] > rhs.data[i])
					return 1;
			}
			return 0;
		}
	};

	constexpr auto f64MaxDecExp = 308;
	constexpr auto f64MinDecExp = (-324);

#define repeat_in_1_18(x) { x(1) x(2) x(3) x(4) x(5) x(6) x(7) x(8) x(9) x(10) x(11) x(12) x(13) x(14) x(15) x(16) x(17) x(18) }
	constexpr auto eBit = static_cast<uint8_t>('E' ^ 'e');

	template<jsonifier::concepts::float_t value_type, typename iterator_type> JSONIFIER_INLINE bool parseFloat(value_type& value, iterator_type&& iter) noexcept {
		constexpr auto isVolatile				= std::is_volatile_v<std::remove_reference_t<decltype(value)>>;
		string_view_ptr sigCut					= nullptr;
		[[maybe_unused]] string_view_ptr sigEnd = nullptr;
		string_view_ptr dotPos					= nullptr;
		uint32_t fracZeros						= 0;
		uint64_t sig							= 0;
		int32_t exp								= 0;
		bool expSign;
		int32_t expSig = 0;
		int32_t expLit = 0;
		uint64_t numTmp;
		string_view_ptr tmp;
		string_view_ptr hdr = iter;
		bool sign			= (*hdr == '-');
		iter += sign;
		auto applySign = [&](auto&& value) -> value_type {
			return sign ? -static_cast<value_type>(value) : static_cast<value_type>(value);
		};

		sig = uint64_t(numberSubTable[static_cast<uint8_t>(*iter)]);
		if (sig > 9) {
			if constexpr (std::integral<value_type>) {
				return false;
			} else if (*iter == 'n' && iter[1] == 'u' && iter[2] == 'l' && iter[3] == 'l') {
				iter += 4;
				value = std::numeric_limits<value_type>::quiet_NaN();
				return true;
			} else if ((*iter | eBit) == 'n' && (iter[1] | eBit) == 'a' && (iter[2] | eBit) == 'n') {
				iter += 3;
				value = sign ? -std::numeric_limits<value_type>::quiet_NaN() : std::numeric_limits<value_type>::quiet_NaN();
				return true;
			} else {
				return false;
			}
		}
#define expr_intg(i) \
	if ((numTmp = numberSubTable[static_cast<uint8_t>(iter[i])]) <= 9) [[likely]] \
		sig = numTmp + sig * 10; \
	else { \
		if constexpr (i > 1) { \
			if (*iter == zero) \
				return false; \
		} \
		goto digi_sepr_##i; \
	}
		repeat_in_1_18(expr_intg);
#undef expr_intg
		if (*iter == zero) [[unlikely]] {
			return false;
		}
		iter += 19;
		if (!digiIsDigitOrFp(*iter)) {
			value = static_cast<value_type>(sig);
			if constexpr (!std::is_unsigned_v<value_type>) {
				if constexpr (isVolatile) {
					value = value * (sign ? -1 : 1);
				} else {
					value *= sign ? -1 : 1;
				}
			}
			return true;
		}
		goto digi_intg_more;
#define expr_sepr(i) \
	digi_sepr_##i : if ((!digiIsFp(iter[i]))) [[likely]] { \
		iter += i; \
		value = applySign(sig); \
		return true; \
	} \
	dotPos = iter + i; \
	if ((iter[i] == '.')) [[likely]] { \
		if (sig == 0) \
			while (iter[fracZeros + i + 1] == zero) \
				++fracZeros; \
		goto digi_frac_##i; \
	} \
	iter += i; \
	sigEnd = iter; \
	goto digi_exp_more;
		repeat_in_1_18(expr_sepr)
#undef expr_sepr
#define expr_frac(i) \
	digi_frac_##i : if ((numTmp = numberSubTable[static_cast<uint8_t>(iter[i + 1 + fracZeros])]) <= 9) [[likely]] sig = numTmp + sig * 10; \
	else { \
		goto digi_stop_##i; \
	}
			repeat_in_1_18(expr_frac)
#undef expr_frac
				iter += 20 + fracZeros;
		if (uint8_t(numberSubTable[static_cast<uint8_t>(*iter)]) > 9)
			goto digi_frac_end;
		goto digi_frac_more;
#define expr_stop(i) \
	digi_stop_##i : iter += i + 1 + fracZeros; \
	goto digi_frac_end;
		repeat_in_1_18(expr_stop)
#undef expr_stop
			digi_intg_more : static constexpr uint64_t U64_MAX = (std::numeric_limits<uint64_t>::max)();
		if ((numTmp = numberSubTable[static_cast<uint8_t>(*iter)]) < 10) {
			if (!digiIsDigitOrFp(iter[1])) {
				if ((sig < (U64_MAX / 10)) || (sig == (U64_MAX / 10) && numTmp <= (U64_MAX % 10))) {
					sig = numTmp + sig * 10;
					++iter;
					value = static_cast<value_type>(sig);
					if constexpr (!std::is_unsigned_v<value_type>) {
						if constexpr (isVolatile) {
							value = value * (sign ? -1 : 1);
						} else {
							value *= sign ? -1 : 1;
						}
					}
					return true;
				}
			}
		}
		if ((eBit | *iter) == 'e') {
			dotPos = iter;
			goto digi_exp_more;
		}
		if (*iter == '.') {
			dotPos = iter++;
			if (uint8_t(numberSubTable[static_cast<uint8_t>(*iter)]) > 9) {
				return false;
			}
		}
	digi_frac_more:
		sigCut = iter;
		sig += (*iter >= '5');
		while (uint8_t(numberSubTable[static_cast<uint8_t>(*++iter)]) < 10) {
		}
		if (!dotPos) {
			dotPos = iter;
			if (*iter == '.') {
				if (uint8_t(numberSubTable[static_cast<uint8_t>(*++iter)]) > 9) {
					return false;
				}
				while (uint8_t(numberSubTable[static_cast<uint8_t>(*++iter)]) < 10) {
				}
			}
		}
		expSig = static_cast<int32_t>(dotPos - sigCut);
		expSig += (dotPos < sigCut);
		tmp = iter - 1;
		while (*tmp == '0' || *tmp == '.') {
			--tmp;
		}
		if (tmp < sigCut) {
			sigCut = nullptr;
		} else {
			sigEnd = iter;
		}
		if ((eBit | *iter) == 'e')
			goto digi_exp_more;
		goto digi_exp_finish;
	digi_frac_end:
		sigEnd = iter;
		expSig = -int32_t((iter - dotPos) - 1);
		if (expSig == 0)
			return false;
		if ((eBit | *iter) != 'e') [[likely]] {
			if ((expSig < f64MinDecExp - 19)) [[unlikely]] {
				value = applySign(0);
				return true;
			}
			exp = expSig;
			goto digi_finish;
		} else {
			goto digi_exp_more;
		}
	digi_exp_more:
		expSign = (*++iter == '-');
		iter += (*iter == '+' || *iter == '-');
		if (uint8_t(numberSubTable[static_cast<uint8_t>(*iter)]) > 9) [[unlikely]] {
			return false;
		}
		while (*iter == '0') {
			++iter;
		}
		tmp = iter;
		uint8_t c;
		while (uint8_t{ c = static_cast<uint8_t>(numberSubTable[static_cast<uint8_t>(*iter)]) } < 10) {
			++iter;
			expLit = static_cast<int32_t>(c + uint32_t(expLit) * 10);
		}
		if ((iter - tmp >= 6)) [[unlikely]] {
			if (sig == 0 || expSign) {
				value = applySign(0);
				value = static_cast<value_type>(sig);
				return true;
			} else {
				value = applySign(std::numeric_limits<value_type>::infinity());
				return true;
			}
		}
		expSig += expSign ? -expLit : expLit;
	digi_exp_finish:
		if (sig == 0) {
			value = (sign ? -value_type{ 0 } : value_type{ 0 });
			return true;
		}
		if ((expSig < f64MinDecExp - 19)) [[unlikely]] {
			value = (sign ? -value_type{ 0 } : value_type{ 0 });
			return true;
		} else if ((expSig > f64MaxDecExp)) [[unlikely]] {
			value = sign ? -std::numeric_limits<value_type>::infinity() : std::numeric_limits<value_type>::infinity();
			return true;
		}
		exp = expSig;
	digi_finish:

		if constexpr (std::is_same_v<double, value_type>) {
			if (sig < (uint64_t(1) << 53) && std::abs(exp) <= 22) {
				value = static_cast<value_type>(sig);
				if constexpr (isVolatile) {
					if constexpr (!std::is_unsigned_v<value_type>) {
						value = value * (sign ? -1 : 1);
					}
					if (exp >= 0) {
						value = value * powersOfTenFloat[exp];
					} else {
						value = value / powersOfTenFloat[-exp];
					}
				} else {
					if constexpr (!std::is_unsigned_v<value_type>) {
						value *= sign ? -1 : 1;
					}
					if (exp >= 0) {
						value *= powersOfTenFloat[static_cast<uint64_t>(exp)];
					} else {
						value /= powersOfTenFloat[static_cast<uint64_t>(-exp)];
					}
				}

				return true;
			}
		} else {
			if (sig < (uint64_t(1) << 24) && std::abs(exp) <= 8) {
				value = static_cast<value_type>(sig);
				if constexpr (!std::is_unsigned_v<value_type>) {
					value *= sign ? -1 : 1;
				}
				if (exp >= 0) {
					value *= static_cast<value_type>(powersOfTenFloat[static_cast<uint64_t>(exp)]);
				} else {
					value /= static_cast<value_type>(powersOfTenFloat[static_cast<uint64_t>(-exp)]);
				}
				return true;
			}
		}

		if (sig == 0) [[unlikely]] {
			value = value_type(0);
			return true;
		}

		static_assert(std::numeric_limits<value_type>::is_iec559);
		static_assert(std::numeric_limits<value_type>::radix == 2);
		static_assert(std::is_same_v<float, jsonifier::concepts::unwrap_t<value_type>> || std::is_same_v<double, jsonifier::concepts::unwrap_t<value_type>>);
		static_assert(sizeof(float) == 4 && sizeof(double) == 8);

		using Raw						= std::conditional_t<std::is_same_v<float, jsonifier::concepts::unwrap_t<value_type>>, uint32_t, uint64_t>;
		auto sigLeadingZeros			= simd_internal::lzcnt(sig);
		auto sigNorm					= sig << sigLeadingZeros;
		auto sig2Norm					= sig2FromExp10(exp);
		auto sigProduct					= mulhi64(sigNorm, sig2Norm) + 1;
		auto sigProductStartsWith1		= sigProduct >> 63;
		auto mantisa					= sigProduct << (2 - sigProductStartsWith1);
		constexpr uint64_t roundMask	= uint64_t(1) << 63 >> (std::numeric_limits<value_type>::digits - 1);
		constexpr uint32_t exponentBits = ceillog2(std::numeric_limits<value_type>::max_exponent - std::numeric_limits<value_type>::min_exponent + 1);
		constexpr uint32_t mantisaShift = exponentBits + 1 + 64 - 8 * sizeof(Raw);
		int32_t exp2					= static_cast<int32_t>(exp2FromExp10(exp) + static_cast<uint32_t>(-static_cast<int64_t>(sigLeadingZeros) + sigProductStartsWith1));

		if (exp2 < std::numeric_limits<value_type>::min_exponent - 1) [[unlikely]] {
			value = sign ? -value_type(0) : value_type(0);
			return true;
		} else if (exp2 > std::numeric_limits<value_type>::max_exponent - 1) [[unlikely]] {
			value = sign ? -std::numeric_limits<value_type>::infinity() : std::numeric_limits<value_type>::infinity();
			return true;
		}

		uint64_t round = 0;
		if (roundMask & mantisa) {
			if (mantisa << (std::numeric_limits<value_type>::digits) == 0) {
				auto sigUpper	  = (mantisa >> (mantisaShift - 1)) | (uint64_t(1) << 63 >> (mantisaShift - 2)) | 1;
				int32_t exp2Upper = exp2 - std::numeric_limits<value_type>::digits;

				big_int_t bigComp{ sigUpper };
				big_int_t bigFull{ sig };
				if (exp >= 0) {
					bigFull.mulPow10(static_cast<uint32_t>(exp));
				} else {
					bigComp.mulPow10(static_cast<uint32_t>(-exp));
				}
				if (exp2Upper >= 0) {
					bigComp.mulPow2(static_cast<uint32_t>(exp2Upper));
				} else {
					bigFull.mulPow2(static_cast<uint32_t>(-exp2Upper));
				}
				auto cmp = bigFull <=> bigComp;
				if (cmp != 0) [[likely]] {
					round = (cmp > 0);
				} else {
					round = (mantisa & (roundMask << 1)) != 0;
				}
			} else if ((exp < pow10SigTableMinExact || exp > pow10SigTableMaxExact) || (mantisa & (roundMask << 1)) ||
				(static_cast<size_t>(simd_internal::tzcnt(sigNorm) + simd_internal::tzcnt(sig2Norm)) <
					128 - std::numeric_limits<value_type>::digits - (2 - sigProductStartsWith1))) {
				round = 1;
			}
		}

		auto num = Raw(sign) << (sizeof(Raw) * 8 - 1) | Raw(mantisa >> mantisaShift) |
			(Raw(exp2 + std::numeric_limits<value_type>::max_exponent - 1) << (std::numeric_limits<value_type>::digits - 1));
		if constexpr (isVolatile) {
			num	  = num + Raw(round);
			value = std::bit_cast<value_type>(num);
		} else {
			num += Raw(round);
			std::memcpy(&value, &num, sizeof(value_type));
		}
		return true;
	}

}