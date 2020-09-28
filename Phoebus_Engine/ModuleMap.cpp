#include "Globals.h"
#include "Application.h"
#include "ModuleMap.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "PhysVehicle3D.h"

ModuleMap::ModuleMap(bool start_enabled) : Module(start_enabled)
{	
}

// Destructor
ModuleMap::~ModuleMap()
{
}

// Called before render is available
bool ModuleMap::Start()
{
	bool ret = true;
	
	memset(waypoint_flags, 0, sizeof(waypoint_flags));//starts the waypoint array from 0
	waypoint_flags[0] = true;//sets the start waypoint to true
	App->scene_intro->lap_time.Start();

	//Floor creation
	btVector3 sizeFloor = { 150.0f, 2.0f, 150.0f };
	btVector3 posFloor = { 0.f, -2.f, 0.f };
	CreateFloor(sizeFloor, posFloor);
	//-----------------------------------------

	//Pillars Creation
	float Map[928] = {
0.509863f, 0.215500f,
	0.509105f, 0.207500f,
	0.509105f, 0.198000f,
	0.509105f, 0.189500f,
	0.509105f, 0.181500f,
	0.509105f, 0.172500f,
	0.509105f, 0.163000f,
	0.511381f, 0.154000f,
	0.513657f, 0.146500f,
	0.515175f, 0.137500f,
	0.518209f, 0.129000f,
	0.522762f, 0.122000f,
	0.525797f, 0.115000f,
	0.529590f, 0.108000f,
	0.534143f, 0.100000f,
	0.539454f, 0.093000f,
	0.544765f, 0.086500f,
	0.551593f, 0.079000f,
	0.558422f, 0.073000f,
	0.566009f, 0.066500f,
	0.573596f, 0.061500f,
	0.579666f, 0.056000f,
	0.586495f, 0.050500f,
	0.594841f, 0.045000f,
	0.602428f, 0.039500f,
	0.610774f, 0.036000f,
	0.619879f, 0.031500f,
	0.628225f, 0.027500f,
	0.637329f, 0.023500f,
	0.645675f, 0.020000f,
	0.655539f, 0.016000f,
	0.667678f, 0.013500f,
	0.676783f, 0.010500f,
	0.688923f, 0.008000f,
	0.698027f, 0.007500f,
	0.707891f, 0.006500f,
	0.715478f, 0.005500f,
	0.727618f, 0.005500f,
	0.736722f, 0.005000f,
	0.748862f, 0.005000f,
	0.760243f, 0.005000f,
	0.772382f, 0.006000f,
	0.783763f, 0.006500f,
	0.796662f, 0.008000f,
	0.808042f, 0.010000f,
	0.820182f, 0.011500f,
	0.830046f, 0.013000f,
	0.840668f, 0.016500f,
	0.851290f, 0.019000f,
	0.861912f, 0.023000f,
	0.871017f, 0.026500f,
	0.880880f, 0.031500f,
	0.888467f, 0.037500f,
	0.896055f, 0.042000f,
	0.902883f, 0.047500f,
	0.910470f, 0.053500f,
	0.916540f, 0.059000f,
	0.925645f, 0.065500f,
	0.930956f, 0.071500f,
	0.939302f, 0.078500f,
	0.943854f, 0.084000f,
	0.948407f, 0.090500f,
	0.953718f, 0.095500f,
	0.958270f, 0.104000f,
	0.962064f, 0.111000f,
	0.965099f, 0.120000f,
	0.967375f, 0.127000f,
	0.968892f, 0.135000f,
	0.969651f, 0.142000f,
	0.970410f, 0.150000f,
	0.970410f, 0.158000f,
	0.968892f, 0.166000f,
	0.966616f, 0.174500f,
	0.963581f, 0.181500f,
	0.959788f, 0.188000f,
	0.955994f, 0.196000f,
	0.951442f, 0.203000f,
	0.945372f, 0.210500f,
	0.941578f, 0.216000f,
	0.935508f, 0.222500f,
	0.928680f, 0.230000f,
	0.922610f, 0.236000f,
	0.915781f, 0.242000f,
	0.907436f, 0.248500f,
	0.899090f, 0.255000f,
	0.890744f, 0.260500f,
	0.883156f, 0.265000f,
	0.874810f, 0.269000f,
	0.864947f, 0.273500f,
	0.855842f, 0.277000f,
	0.846738f, 0.280000f,
	0.837633f, 0.284000f,
	0.827769f, 0.286500f,
	0.818665f, 0.288000f,
	0.805008f, 0.290500f,
	0.792868f, 0.292500f,
	0.778452f, 0.292500f,
	0.765554f, 0.293000f,
	0.751138f, 0.293000f,
	0.737481f, 0.293000f,
	0.723065f, 0.293000f,
	0.708649f, 0.293000f,
	0.695751f, 0.293000f,
	0.682853f, 0.293000f,
	0.669196f, 0.293000f,
	0.654021f, 0.293000f,
	0.641123f, 0.293000f,
	0.627466f, 0.293000f,
		0.618361f, 0.207500f,
		0.618361f, 0.197000f,
		0.620637f, 0.188500f,
		0.621396f, 0.180000f,
		0.622155f, 0.171500f,
		0.622913f, 0.162000f,
		0.625948f, 0.152500f,
		0.627466f, 0.143500f,
		0.630501f, 0.135000f,
		0.635812f, 0.126500f,
		0.641123f, 0.118500f,
		0.650228f, 0.110000f,
		0.656297f, 0.103000f,
		0.666161f, 0.095500f,
		0.676783f, 0.090000f,
		0.685888f, 0.084500f,
		0.696510f, 0.080500f,
		0.708649f, 0.076000f,
		0.720789f, 0.073500f,
		0.733687f, 0.072000f,
		0.748862f, 0.071500f,
		0.763278f, 0.073000f,
		0.777693f, 0.075500f,
		0.789833f, 0.079000f,
		0.802731f, 0.083500f,
		0.814871f, 0.089000f,
		0.823976f, 0.096000f,
		0.834598f, 0.103500f,
		0.842944f, 0.112000f,
		0.848255f, 0.121500f,
		0.851290f, 0.129000f,
		0.852807f, 0.138500f,
		0.852807f, 0.147500f,
		0.849772f, 0.157000f,
		0.844461f, 0.167000f,
		0.836874f, 0.177000f,
		0.831563f, 0.184500f,
		0.823976f, 0.191500f,
		0.817147f, 0.198500f,
		0.809560f, 0.204000f,
		0.796662f, 0.210000f,
		0.789074f, 0.213000f,
		0.776176f, 0.215500f,
		0.764795f, 0.216500f,
		0.750379f, 0.217000f,
		0.735964f, 0.217000f,
		0.724583f, 0.217000f,
		0.711684f, 0.217000f,
		0.696510f, 0.217000f,
		0.682094f, 0.217000f,
		0.670713f, 0.217000f,
		0.659332f, 0.216500f,
		0.647951f, 0.216500f,
		0.633536f, 0.216000f,
		0.619879f, 0.216000f,
		0.621396f, 0.297500f,
		0.622155f, 0.306000f,
		0.622155f, 0.314500f,
		0.622155f, 0.324000f,
		0.622155f, 0.332500f,
		0.622155f, 0.341500f,
		0.622155f, 0.351000f,
		0.622155f, 0.361000f,
		0.622155f, 0.370500f,
		0.622155f, 0.378000f,
		0.622155f, 0.386500f,
		0.622155f, 0.396000f,
		0.622155f, 0.404000f,
		0.622155f, 0.414500f,
		0.622155f, 0.421500f,
		0.622155f, 0.432000f,
		0.622155f, 0.442000f,
		0.622155f, 0.450500f,
		0.622155f, 0.460000f,
		0.622155f, 0.469000f,
		0.622155f, 0.478000f,
		0.622155f, 0.485500f,
		0.622155f, 0.494500f,
		0.622155f, 0.504000f,
		0.622155f, 0.512000f,
		0.622155f, 0.522000f,
		0.622155f, 0.531500f,
		0.622155f, 0.541000f,
		0.622155f, 0.549000f,
		0.622155f, 0.557500f,
		0.622155f, 0.566000f,
		0.622155f, 0.576500f,
		0.622155f, 0.584500f,
		0.622155f, 0.593000f,
		0.622155f, 0.602500f,
		0.628983f, 0.611000f,
		0.628983f, 0.620500f,
		0.628983f, 0.629000f,
		0.628983f, 0.639000f,
		0.628983f, 0.648000f,
		0.628983f, 0.655000f,
		0.628983f, 0.667000f,
		0.628983f, 0.673000f,
		0.627466f, 0.683500f,
		0.625190f, 0.693000f,
		0.622913f, 0.701500f,
		0.619120f, 0.711000f,
		0.614568f, 0.719500f,
		0.609256f, 0.729000f,
		0.602428f, 0.737500f,
		0.594082f, 0.746500f,
		0.584977f, 0.755500f,
		0.574355f, 0.763500f,
		0.564492f, 0.770500f,
		0.553111f, 0.778000f,
		0.541730f, 0.785000f,
		0.529590f, 0.791500f,
		0.519727f, 0.796500f,
		0.507587f, 0.802000f,
		0.496206f, 0.806000f,
		0.482549f, 0.810000f,
		0.467375f, 0.813500f,
		0.449924f, 0.814500f,
		0.434750f, 0.815000f,
		0.419575f, 0.815000f,
		0.402124f, 0.814000f,
		0.383156f, 0.813000f,
		0.364947f, 0.811500f,
		0.351290f, 0.809000f,
		0.333839f, 0.806500f,
		0.317147f, 0.804000f,
		0.301973f, 0.802000f,
		0.288316f, 0.799000f,
		0.274659f, 0.795500f,
		0.262519f, 0.792000f,
		0.252656f, 0.788000f,
		0.244310f, 0.784500f,
		0.232929f, 0.780000f,
		0.220030f, 0.772500f,
		0.210926f, 0.766000f,
		0.202580f, 0.759500f,
		0.195751f, 0.750500f,
		0.190440f, 0.741500f,
		0.185888f, 0.733000f,
		0.182094f, 0.725000f,
		0.179059f, 0.715000f,
		0.176783f, 0.705500f,
		0.173748f, 0.695500f,
		0.171472f, 0.686500f,
		0.168437f, 0.677500f,
			0.166920f, 0.667000f,
			0.164643f, 0.657500f,
			0.163126f, 0.648500f,
			0.162367f, 0.640000f,
			0.161609f, 0.629000f,
			0.160091f, 0.622500f,
			0.159332f, 0.612500f,
			0.158574f, 0.603000f,
			0.158574f, 0.594000f,
			0.158574f, 0.585000f,
			0.158574f, 0.575500f,
			0.158574f, 0.566500f,
			0.157815f, 0.557000f,
			0.157815f, 0.549500f,
			0.157815f, 0.540500f,
			0.157815f, 0.531500f,
			0.157815f, 0.521000f,
			0.157815f, 0.512500f,
			0.157815f, 0.503500f,
			0.157815f, 0.495500f,
			0.160091f, 0.486000f,
			0.161609f, 0.477000f,
			0.160850f, 0.469000f,
			0.160850f, 0.460500f,
			0.160091f, 0.450000f,
			0.156297f, 0.442000f,
			0.149469f, 0.432000f,
			0.141123f, 0.423500f,
			0.130501f, 0.416000f,
			0.119879f, 0.410000f,
			0.107739f, 0.406000f,
			0.095599f, 0.403000f,
			0.081942f, 0.398500f,
			0.069803f, 0.393500f,
			0.059181f, 0.389500f,
			0.049317f, 0.383500f,
			0.038695f, 0.376500f,
			0.028832f, 0.369500f,
			0.022003f, 0.362000f,
			0.015175f, 0.353500f,
			0.009105f, 0.344000f,
			0.003794f, 0.334500f,
			0.000759f, 0.325500f,
			0.000000f, 0.317500f,
			0.000000f, 0.308000f,
			0.001517f, 0.298000f,
			0.006070f, 0.288000f,
			0.009863f, 0.280500f,
			0.014416f, 0.272500f,
			0.022003f, 0.263000f,
			0.029590f, 0.254500f,
			0.039454f, 0.248500f,
			0.047800f, 0.242500f,
			0.054628f, 0.239000f,
			0.066009f, 0.233000f,
			0.080425f, 0.226000f,
			0.094082f, 0.222000f,
			0.106980f, 0.219000f,
			0.121396f, 0.217000f,
			0.135053f, 0.215500f,
			0.151745f, 0.216000f,
			0.163126f, 0.216000f,
			0.174507f, 0.216000f,
			0.189681f, 0.216000f,
			0.207132f, 0.216000f,
			0.218513f, 0.216000f,
			0.229894f, 0.216000f,
			0.245068f, 0.216000f,
			0.257967f, 0.216000f,
			0.273141f, 0.216000f,
			0.287557f, 0.216000f,
			0.298938f, 0.216000f,
			0.313354f, 0.216000f,
			0.327769f, 0.216000f,
			0.340668f, 0.216000f,
			0.355083f, 0.216000f,
			0.367982f, 0.216000f,
			0.383915f, 0.216000f,
			0.396055f, 0.216000f,
			0.407436f, 0.216000f,
			0.422610f, 0.216000f,
			0.437026f, 0.216000f,
			0.452959f, 0.216000f,
			0.465099f, 0.216000f,
			0.478756f, 0.216000f,
			0.497724f, 0.216000f,
			0.507587f, 0.292000f,
			0.507587f, 0.299500f,
			0.507587f, 0.306000f,
			0.507587f, 0.314500f,
			0.507587f, 0.324000f,
			0.507587f, 0.334000f,
			0.507587f, 0.342500f,
			0.507587f, 0.350000f,
			0.507587f, 0.360500f,
			0.507587f, 0.369500f,
			0.507587f, 0.378000f,
			0.507587f, 0.387500f,
			0.507587f, 0.397000f,
			0.507587f, 0.406000f,
			0.507587f, 0.415000f,
			0.507587f, 0.424000f,
			0.507587f, 0.432500f,
			0.507587f, 0.442000f,
			0.507587f, 0.450500f,
			0.507587f, 0.460000f,
			0.507587f, 0.468500f,
			0.507587f, 0.477500f,
			0.507587f, 0.486500f,
			0.507587f, 0.495000f,
			0.507587f, 0.504500f,
			0.507587f, 0.513500f,
			0.507587f, 0.523000f,
			0.507587f, 0.532000f,
			0.507587f, 0.541000f,
			0.507587f, 0.549500f,
			0.507587f, 0.558000f,
			0.507587f, 0.567000f,
			0.507587f, 0.577000f,
			0.507587f, 0.586000f,
			0.507587f, 0.595000f,
			0.507587f, 0.604000f,
			0.507587f, 0.612000f,
				0.514416f, 0.620500f,
				0.510622f, 0.630000f,
				0.506829f, 0.639500f,
				0.502276f, 0.648000f,
				0.497724f, 0.656000f,
				0.492413f, 0.664500f,
				0.485584f, 0.672000f,
				0.479514f, 0.679500f,
				0.470410f, 0.687500f,
				0.461305f, 0.694000f,
				0.450683f, 0.702000f,
				0.437785f, 0.707500f,
				0.422610f, 0.712000f,
				0.408194f, 0.715500f,
				0.391502f, 0.715500f,
				0.377086f, 0.712500f,
				0.363429f, 0.706500f,
				0.351290f, 0.699000f,
				0.339909f, 0.690000f,
				0.330046f, 0.678000f,
				0.321700f, 0.668000f,
				0.315630f, 0.659000f,
				0.310319f, 0.649000f,
				0.305008f, 0.639500f,
				0.300455f, 0.628000f,
				0.297420f, 0.618500f,
				0.293627f, 0.607500f,
				0.289833f, 0.595000f,
				0.286039f, 0.585500f,
				0.283005f, 0.575000f,
				0.280728f, 0.562500f,
				0.279211f, 0.551500f,
				0.279211f, 0.540500f,
				0.278452f, 0.529000f,
				0.278452f, 0.519500f,
				0.278452f, 0.510500f,
				0.278452f, 0.503500f,
				0.278452f, 0.494500f,
				0.278452f, 0.486000f,
				0.278452f, 0.476500f,
				0.278452f, 0.468500f,
				0.278452f, 0.459500f,
				0.278452f, 0.451500f,
				0.278452f, 0.442000f,
				0.278452f, 0.434000f,
				0.274659f, 0.424500f,
				0.267071f, 0.415000f,
				0.257967f, 0.406000f,
				0.249621f, 0.397500f,
				0.240516f, 0.390500f,
				0.231411f, 0.383500f,
				0.218513f, 0.376000f,
				0.205615f, 0.371000f,
				0.191199f, 0.365000f,
				0.177542f, 0.361000f,
				0.164643f, 0.356500f,
				0.152504f, 0.349000f,
				0.141123f, 0.340500f,
				0.132777f, 0.331500f,
				0.128225f, 0.322000f,
				0.129742f, 0.310000f,
				0.138088f, 0.301000f,
				0.147951f, 0.295500f,
				0.162367f, 0.293000f,
				0.174507f, 0.292500f,
				0.189681f, 0.292000f,
				0.205615f, 0.292000f,
				0.218513f, 0.292000f,
				0.231411f, 0.292000f,
				0.245827f, 0.292000f,
				0.258725f, 0.292000f,
				0.272382f, 0.292000f,
				0.286798f, 0.292000f,
				0.298938f, 0.292000f,
				0.312595f, 0.292000f,
				0.325493f, 0.292000f,
				0.337633f, 0.292000f,
				0.355083f, 0.292000f,
				0.367982f, 0.292000f,
				0.380880f, 0.292000f,
				0.395296f, 0.292000f,
				0.408953f, 0.292000f,
				0.423369f, 0.292000f,
				0.437026f, 0.292000f,
				0.449924f, 0.292000f,
				0.465857f, 0.292000f,
				0.478756f, 0.292000f,
				0.493171f, 0.292000f


	};
	int sizeMap = 928;
	vec2 dist_origin = { 115.0f,120.0f };

	for (int i = 0; i < sizeMap / 2; i++)
	{
		pillar[i].pillar_size = { 1.3f, 3.0f, 1.0f };
		pillar[i].pillars_pos = { Map[i * 2 + 0] * DISTANCE_X_RATIO,  1.7f,  Map[i * 2 + 1] * DISTANCE_Z_RATIO };
	}
	CreatePillars(pillar, sizeMap, dist_origin);
	//-------------------------------------------

	//Ramps Creation
	btVector3 sizeRamps = { 1.0f, 24.0f ,21.0f };
	btVector3 posRamp1 = { 20.0f, 0.5f ,-44.0f };	
	btVector3 posRamp2 = { -25.0f, 0.5f ,-44.0f };
	float angleRamp = 85.0f;

	CreateRamps(sizeRamps, posRamp1, angleRamp);
	CreateRamps(sizeRamps, posRamp2, -angleRamp);
	//----------------------------------------------

	//Fan Creation	
	fan.fan_pos = { -68.0f, 10.0f, 70.0f };
	fan.fan_size1 = { 25, 0.4f, 2.0f };
	fan.fan_size2 = { 2.0f, 0.4f, 25.0f };
	fan.joint_size = { 0.1, 0.025, 0.1 };
	fan.rotation.setRotation(btVector3(1, 0, 0), 80);
	fan.mass = 150;
	fan.body=CreateFan(fan);
	//---------------------------------------------

	//Wreckingball Creation
	wrecking_ball.base_size = { 2.0f, 0.2f, 7.0f };
	wrecking_ball.ball_size = 2.0f;	
	wrecking_ball.base_pos = { -76.0f, 15.0f, -44.0f };
	wrecking_ball.ball_pos = { -76.0f, 0.0f, -36.0f };
	wrecking_ball.mass = 1000.0f;
	wrecking_ball.body = CreateWreckingBall(wrecking_ball);
	//----------------------------------------------	

	//We create the waypoint sensors
	last_waypoint = CreateSensor({ -2.f, 7.5f, -6.f }, { -2.f, 4.5f, 0.f }, { 12.0f,5.0f,1.0f },180.f);
	CreateSensor({ 41.0f, 7.5f, -108.5f }, { 35.0f, 4.5f, -108.5f }, { 9.0f,5.0f,1.0f },-90);
	CreateSensor({ -56, 7.5f, -44.f }, { -50, 4.5f, -44.f }, { 12.0f,5.0f,1.0f }, 90);
	CreateSensor({ -29, 7.5f, 109.f }, { -35, 4.5f, 109.f }, { 15.0f,5.0f,1.0f }, -90);	
	//-----------------------------------------------

	return ret;

}

update_status ModuleMap::Update(float dt)
{
	for (uint n = 0; n < primitives.Count(); n++)
		primitives[n]->Update();
	
	return UPDATE_CONTINUE;
}

// Update: draw background
update_status ModuleMap::PostUpdate(float dt)
{
	//Rendering static primitives
	for (uint n = 0; n < primitives.Count(); n++)
	{
		primitives[n]->Render();
	}

	//Rendering dynamic primitives

	//Fan primitives
	mat4x4 mat;
	fan.body->getWorldTransform().getOpenGLMatrix(mat.M);

	Cube c1({ fan.fan_size1.x,fan.fan_size1.y, fan.fan_size1.z }, fan.mass);
	c1.SetPos(fan.fan_pos.x, fan.fan_pos.y, fan.fan_pos.z);
	c1.color = Green;
	c1.SetRotation(fan.rotation.getX(), { fan.rotation.getY(), fan.rotation.getZ(), fan.rotation.getW()});
	c1.transform = mat;
	c1.Render();

	Cube c2({ fan.fan_size2.x,fan.fan_size2.y, fan.fan_size2.z }, fan.mass);
	c2.SetPos(fan.fan_pos.x, fan.fan_pos.y, fan.fan_pos.z);
	c2.color = Green;
	c2.SetRotation(fan.rotation.getX(), { fan.rotation.getY(), fan.rotation.getZ(), fan.rotation.getW()});
	c2.transform = mat;
	c2.Render();

	//Wreckingball primitive
	mat4x4 mat1;
	wrecking_ball.body->getWorldTransform().getOpenGLMatrix(mat1.M);
	Sphere s1(wrecking_ball.ball_size, wrecking_ball.mass);	
	s1.color = White;
	s1.transform = mat1;
	s1.Render();

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleMap::CleanUp()
{
	LOG("Destroying map");

	//TODO clean all the waypoints
	for (int i = waypoints.Count() - 1; i >= 0; i--)
	{
		delete waypoints[i];
	}

	waypoints.Clear();

	// Clear motions 
	for (p2List_item<btDefaultMotionState*>* item = motions.getFirst(); item; item = item->next)
		delete item->data;

	motions.clear();

	// Clear shapes 
	for (p2List_item<btCollisionShape*>* item = shapes.getFirst(); item; item = item->next)
		delete item->data;

	shapes.clear();
	
	primitives.Clear();

	return true;
}

// Detects sensors' collisions and updates the waypoints
void ModuleMap::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	PhysBody3D* body;
	if (body1->GetBody() != App->player->vehicle->GetBody())
		body = body1;
	else body = body2;

	if (body != last_waypoint)//only executes code if the last waypoint visited is not the current one
	{
		int i;
		for (i = 0; i < waypoints.Count(); i++)//determines the id of whe current waypoint
		{
			if (body == waypoints[i])
				break;
		}

		if (i == 0)//if its the first element of the list
		{
			if (waypoint_flags[waypoints.Count() - 1] == true)
			{
				memset(waypoint_flags, 0, sizeof(waypoint_flags));//starts the waypoint array from 0
				laps++;
				App->scene_intro->CalculateBestLap(&App->scene_intro->lap_time);
				App->scene_intro->lap_time.Start();				
				last_waypoint = body;
				waypoint_flags[0] = true;
				App->audio->PlayFx(1);
				for (int i = 0; i < waypoints.Count(); i++) {
					waypoints[i]->parentPrimitive->color = White;
				}

				waypoints[0]->parentPrimitive->color = Green;
			}
		}
		else
		{
			if (waypoint_flags[i - 1] == true)
			{
				App->audio->PlayFx(2);
				body->parentPrimitive->color = Green;
				waypoint_flags[i] = true;
				last_waypoint = body;
			}
		}

	}
	
}

int ModuleMap::GetLaps() const
{
	return laps;
}

PhysBody3D* ModuleMap::GetLastWaypoint() const
{
	return last_waypoint;
}

void ModuleMap::ResetGame()
{
	memset(waypoint_flags, 0, sizeof(waypoint_flags));//starts the waypoint array from 0
	for (int i = 0; i < waypoints.Count(); i++)
	{
		waypoints[i]->parentPrimitive->color = White;

	}
	last_waypoint = waypoints[0];
	waypoint_flags[0] = true;
	App->player->TpToLastWaypoint();
	App->scene_intro->lap_time.Start();
	laps = 0;
}

btRigidBody* ModuleMap::CreateFloor(const btVector3 size, const btVector3 pos) {

	// Big rectangle as ground
	btCollisionShape* colShape = new btBoxShape(btVector3(size));
	shapes.add(colShape);
	mat4x4 glMatrix = IdentityMatrix;
	glMatrix.translate(pos.getX(),pos.getY(),pos.getZ());
	btTransform startTransform;
	startTransform.setFromOpenGLMatrix(&glMatrix);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	motions.add(myMotionState);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);

	btRigidBody* body = new btRigidBody(rbInfo);

	App->physics->AddBodyToWorld(body);

	//Floor primitive
	Cube* floor = new Cube({ size.getX()*2 , size.getY() * 2, size.getZ() * 2 }, 0.0f);
	floor->SetPos(pos.getX(), pos.getY(), pos.getZ());
	floor->color = Gray;
	primitives.PushBack(floor);

	return(body);
}

btRigidBody* ModuleMap::CreatePillars(const Pillars Pillar[], const int size, const vec2 dist_origin)
{	
	btCollisionShape* mapShape = new btCylinderShape(btVector3(pillar[0].pillar_size.x * 0.5f, pillar[0].pillar_size.y * 0.5f, pillar[0].pillar_size.z * 0.5f));
	shapes.add(mapShape);

	btTransform mapTrans;
	mapTrans.setIdentity();
	btDefaultMotionState* mapMotionState = new btDefaultMotionState(mapTrans);
	motions.add(mapMotionState);
	btRigidBody::btRigidBodyConstructionInfo mapInfo(0.0f, mapMotionState, mapShape, { 0,0,0 });

	btRigidBody* body = nullptr;
	for (int i = 0; i < size / 2; i++)
	{
		body = new btRigidBody(mapInfo);
		body->getWorldTransform().setOrigin(btVector3(pillar[i].pillars_pos.x - dist_origin.x, pillar[i].pillars_pos.y, pillar[i].pillars_pos.z - dist_origin.y));
		App->physics->AddBodyToWorld(body);
	}

	//Primitives of pillars
	for (int i = 0; i < size / 2; i++)
	{
		Cylinder* s = new Cylinder(1.3f * 0.5f, 3.0f, 0.0f);
		s->SetPos(pillar[i].pillars_pos.x - 115.0f, pillar[i].pillars_pos.y, pillar[i].pillars_pos.z - 120.0f);
		s->SetRotation(90.f, { 0.0f,0.0f,1.0f });

		//We alternate color between pillars
		if (i % 2 == 0)
			s->color = Red;
		else
			s->color = White;

		primitives.PushBack(s);
	}

	return(body);
}

btRigidBody* ModuleMap::CreateRamps(const btVector3 size, const btVector3 pos, const float angle)
{
	btCollisionShape* rampShape = new btBoxShape(btVector3(size.getX() * 0.5f, size.getY() * 0.5f, size.getZ() * 0.5f));
	shapes.add(rampShape);

	mat4x4 rampMatrix = IdentityMatrix;
	rampMatrix.translate(pos.getX(), pos.getY(), pos.getZ());
	rampMatrix.rotate(angle, { 0,0,1 });
	btTransform rampTransform;
	rampTransform.setFromOpenGLMatrix(&rampMatrix);

	btDefaultMotionState* rampMotionState = new btDefaultMotionState(rampTransform);
	motions.add(rampMotionState);
	btRigidBody::btRigidBodyConstructionInfo rampInfo(0.0f, rampMotionState, rampShape);

	btRigidBody* body = new btRigidBody(rampInfo);
	App->physics->AddBodyToWorld(body);

	// Primitive
	Cube* ramp1 = new Cube(vec3(size.getX(), size.getY(), size.getZ()), 0.0f);
	ramp1->SetPos(pos.getX(), pos.getY(), pos.getZ());
	ramp1->SetRotation(angle, { 0,0,1 });
	ramp1->color = Cyan;
	primitives.PushBack(ramp1);

	return(body);
}

btRigidBody* ModuleMap::CreateFan(Fan fan) {
		
	btCollisionShape* shape1 = new btBoxShape(btVector3(fan.fan_size1.x * 0.5f, fan.fan_size1.y * 0.5f, fan.fan_size1.z * 0.5f));
	shapes.add(shape1);
	btCollisionShape* shape2 = new btCylinderShape(btVector3(fan.joint_size.x, fan.joint_size.y, fan.joint_size.z));
	shapes.add(shape2);
	btCollisionShape* shape3 = new btBoxShape(btVector3(fan.fan_size2.x * 0.5f, fan.fan_size2.y * 0.5f, fan.fan_size2.z * 0.5f));
	shapes.add(shape3);

	btCompoundShape* fanShape = new btCompoundShape();
	fanShape->addChildShape(btTransform::getIdentity(), shape1);
	fanShape->addChildShape(btTransform::getIdentity(), shape2);
	fanShape->addChildShape(btTransform::getIdentity(), shape3);
	shapes.add(fanShape);
	
	btVector3 localInertia;
	fanShape->calculateLocalInertia(fan.mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo fanInfo(fan.mass, 0, fanShape, localInertia);
	fanInfo.m_startWorldTransform.setOrigin(btVector3(fan.fan_pos.x, fan.fan_pos.y, fan.fan_pos.z));
	fanInfo.m_startWorldTransform.setRotation(fan.rotation);

	btRigidBody* body = new btRigidBody(fanInfo);
	App->physics->AddBodyToWorld(body);
	body->setLinearFactor(btVector3(0, 0, 0));
	
	btHingeConstraint* hinge = App->physics->AddConstraintHinge(body, btVector3(0, 0, 0), btVector3(0, 1, 0), true);	
	hinge->enableAngularMotor(true, -2.0f, 500);

	return body;
}

btRigidBody* ModuleMap::CreateWreckingBall(WreckingBall wreckingball) 
{
	btCollisionShape* shape1 = new btBoxShape(wreckingball.base_size);
	shapes.add(shape1);
	btCollisionShape* shape2 = new btSphereShape(wreckingball.ball_size);
	shapes.add(shape2);

	btTransform frameInA;
	frameInA = btTransform::getIdentity();
	btVector3 localInertia;
	shape2->calculateLocalInertia(wreckingball.mass, localInertia);

	btRigidBody::btRigidBodyConstructionInfo baseInfo(0.0f, 0, shape1, localInertia);
	baseInfo.m_startWorldTransform.setOrigin(wreckingball.base_pos);

	btRigidBody::btRigidBodyConstructionInfo ballInfo(500.0f, 0, shape2, { 100,0,0 });
	ballInfo.m_startWorldTransform.setOrigin(wreckingball.ball_pos);

	btRigidBody* body1 = new btRigidBody(baseInfo);
	btRigidBody* body2 = new btRigidBody(ballInfo);
	body2->setAngularVelocity({-8,0,0 });

	App->physics->AddBodyToWorld(body1);
	App->physics->AddBodyToWorld(body2);

	btTypedConstraint* p2p = App->physics->AddConstraintP2P(body1, body2, { 0,0,0 }, { 0,12.0f,0 });
	
	// Primitive of Wrecking ball base
	Cube* base = new Cube(vec3(wreckingball.base_size.getX()*2, wreckingball.base_size.getY() * 2, wreckingball.base_size.getZ() * 2));
	base->SetPos(wreckingball.base_pos.getX(), wreckingball.base_pos.getY(), wreckingball.base_pos.getZ());
	base->color = Cyan;
	primitives.PushBack(base);	

	return body2;
}

//this function creates a sensor/waypoint
//the rigid body rotation takes the Up axis as the rotation axis
PhysBody3D* ModuleMap::CreateSensor(vec3 shapePos,vec3 rbPos,btVector3 rbHalfDimensions,float rbRotation)
{
	Cube* cub = new Cube({ 1.0f, 1.0f, 1.0f }, 0.0f);
	cub->SetPos(shapePos.x,shapePos.y,shapePos.z);
	cub->color = White;
	mat4x4 glMatrix = IdentityMatrix;
	glMatrix.translate(rbPos.x,rbPos.y,rbPos.z);//translation of the box
	glMatrix.rotate(rbRotation, { 0,-1,0 });//rotation of the box
	btCollisionShape* shap = new btBoxShape(rbHalfDimensions);//measures of the box
	PhysBody3D* bod = new PhysBody3D();

	bod->SetBody(shap, cub, 0.0f);
	bod->SetTransform(glMatrix.M);
	bod->SetAsSensor(true);
	bod->collision_listeners.PushBack(this);
	primitives.PushBack(cub);
	waypoints.PushBack(bod);

	return bod;
}