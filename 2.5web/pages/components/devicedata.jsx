import './style.css';
import { useEffect } from 'preact/hooks';


export function DeviceData(props) {

	useEffect(() => {

	}, [])

	return (
		<table class="device-data">
			<tr>
				<th>Device Data</th>
				<th></th>
			</tr>
			<tr>
				<td>Built Date</td>
				<td>{props.builtDate}</td>
			</tr>
			<tr> 
				<td>Run Time</td>
				<td>{props.runTime}</td>
			</tr>
			<tr>
				<td>Time Keeping</td>
				<td>{props.timeKeeping}</td>
			</tr>
			<tr>
				<td>Reset Count</td>
				<td>{props.resetCount}</td>
			</tr>
			<tr>
				<td colspace="2"><hr /></td>
			</tr>
			<tr>
				<td>Device Name</td>
				<td>{props.deviceName}</td>
			</tr>
			<tr>
				<td>Hostname</td>
				<td>{props.hostname}</td>
			</tr>
			<tr>
				<td>Network</td>
				<td>{props.network}/td>
			</tr>
			<tr>
				<td>Mac Address</td>
				<td>{props.macAddress}</td>
			</tr>

			<tr>
				<td></td>
			</tr>
			<tr>
				<td>MCU ChipID</td>
				<td>Data 4</td>
			</tr>
			<tr>
				<td>CPU Freq</td>
				<td>Data 4</td>
			</tr>
			<tr>
				<td>Flash Size</td>
				<td>Data 4</td>
			</tr>
			<tr>
				<td>Sketch Size</td>
				<td>Data 4</td>
			</tr>
		</table>
		</div>
	);
}
