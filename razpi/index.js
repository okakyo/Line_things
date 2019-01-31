const USER_SERVICE_UUID='cc60d7c4-f126-4cbb-a8d3-1abed3770294';

const WRITE_CHARACTERISTIC_UUID='';
const NOTIFY_CHARACTERISTIC_UUID='';

const PSDI_SERVICE_UUID='e625601e-9e55-4597-a598-76018a0d293d';
const PSDI_CHARACTERISTIC_UUID='26e2b12b-85f0-4f3f-9fdd-91d114270e6e';

const DEVICE_NAME='line_things';

const bleno=require('bleno');
const onoff=require('onoff');

const Characteristic=bleno.Characteristic;
const PrimaryService=bleno.PrimaryService;

const gpio=onoff.Gpio;

const writeCharacteristic=new Characteristic({
	uuid:WRITE_CHARACTERISTIC_UUID,
	properties:['write'],
	onWriteRequest:(data,offset,withoutResponse,callback)=>{
		console.log(`onoff=${data[0]}`)

		var led=new gpio(12,"out");
		led.writeSync(data[0]);
		callback(Characteristic.RESULT_SUCCESS);
	}
});

let onOff=0;

const notifyCharacteristic=new Characteristic({
	uuid:NOTIFY_CHARACTERISTIC_UUID,
	properties:['nogify'],
	onSubscribe:(maxSize,callback)=>{
	console.log('subscribe');
	setInterval(function(){
	onOff^=1;
	callback(new Buffer([onOff]));
	},2000);
	}
});

const psdiCharacteristic=new Characteristic({
	uuid:PSDI_CHARACTERISTIC_UUID,
	properties:['read'],
	onReadRequest:(offset,callback)=>{
		console.log('PSDI read');
		const result=Characteristic.RESULT_SUCCESS;
		const data=new Buffer.from('PSDI read');
		callback(result,data);
	}
});

bleno.on('stateChange',(state)=>{
	console.log(`on->stateChange:${state}`);
	if(state==='powerOn'){
		bleno.startAdvertising(DEVICE_NAME,[USER_SERVICE_UUID]);}
	else
		bleno.stopAdvertising();
});
bleno.on('advertisingStart',(error)=>{
	console.log('on -> advertisingStart: ${(error?"error"+error:"Success")}')
	if(error) return;
	
	const userService =PrimaryService({
		uuid:USER_SERVICE_UUID,
		characteristic:[writeCharacteristic,notifyCharacteristic]
	});

	const psdiService=new PrimaryService({
		uuid:PSDI_SERVICE_UUID,
		characteristic:[psdiCharacteristic]
	});
	bleno.setService([userService,psdiService]);
});


