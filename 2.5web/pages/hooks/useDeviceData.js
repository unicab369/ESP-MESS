import { useState } from 'preact/hooks';
import { DeviceData } from '../models'

export const useDeviceData = () => {
	const [deviceData, setDeviceData] = useState(null)

	const refreshDeviceData = () => {
		const dummyDeviceData = new DeviceData({
			builtDate: '111111',
			runTime: '111111',
			timeKeeping: '111111',
			resetCount: '111111',
			deviceName: '111111',
			hostname: '111111',
			network: '111111',
			macAddress: '11111111',
			mcuChipId: '1111111',
			cpuFrequency: '1111111',
			flashSize: '111111',
			sketchSize: '1111111'
		})
		return new Promise((resolve, reject) => {
			setDeviceData(dummyDeviceData)
			resolve(dummyDeviceData)
		})
	}

	return {
		deviceData,
		refreshDeviceData,
	}
}