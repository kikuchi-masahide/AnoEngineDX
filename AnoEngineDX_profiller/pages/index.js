import Head from 'next/head'
import Body from '../components/body'
import styles from '../styles/Home.module.css'

export default function Home() {
  return (
    <div className={styles.container}>
      <Head>
        <title>profiller</title>
        <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css" crossOrigin='anonymous'></link>
      </Head>
      <Body/>
    </div>
  )
}
