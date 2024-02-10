import './style.css';
import { useEffect } from 'preact/hooks';


export function Page() {

	useEffect(() => {

	}, [])

	return (
		<div class='page'>
			<div class="page-title">
				<h1>ESP-MESS V3.0.2</h1>
			</div>
			<div class="page-content">	
				<table class="device-data">
					<tr>
						<th>Header 1</th>
						<th></th>
					</tr>
					<tr>
						<td>Built Date</td>
						<td>111111</td>
					</tr>
					<tr> 
						<td>Run Time</td>
						<td>Data 4</td>
					</tr>
					<tr>
						<td>Time Keeping</td>
						<td>Data 4</td>
					</tr>
					<tr>
						<td>Reset Count</td>
						<td>Data 4</td>
					</tr>

					<tr>
						<td></td>
					</tr>
					<tr>
						<td>Device Name</td>
						<td>Data 4</td>
					</tr>
					<tr>
						<td>Hostname</td><td>Data 4</td></tr>
					<tr>
						<td>Network</td>
						<td>Data 4</td>
					</tr>
					<tr>
						<td>Mac Address</td>
						<td>Data 4</td>
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

				<div class="ip-entry">
					<label for="ipAddress">IP Address</label>
					<input
						type="text"
						id="ipAddress" 
						oninput="validateIP(this)"
						placeholder="192.168.4.1"
					/>
					<button onclick="saveDevIP()">Save</button>
					<button >?</button>
				</div>

				<div class="controls">
					<button class="pure-button WidthFull" onclick="sendRequest()">TOGGLE IO</button>
					<button class="pure-button WidthFull" onclick="navToAbout()">About</button>
				</div>

			</div>
		</div>
	);
}
