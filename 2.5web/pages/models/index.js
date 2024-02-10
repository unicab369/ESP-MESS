export class DeviceData {

	constructor(rawData) {
		this.builtDate = rawData.builtDate
		this.runTime = rawData.runTime
		this.timeKeeping = rawData.timeKeeping
		this.resetCount = rawData.resetCount
		this.deviceName = rawData.deviceName
		this.hostname = rawData.hostname
		this.network = rawData.network
		this.macAddress = rawData.macAddress
		this.mcuChipId = rawData.mcuChipId
		this.cpuFrequency = rawData.cpuFrequency
		this.flashSize = rawData.flashSize
		this.sketchSize = rawData.sketchSize
	}
}